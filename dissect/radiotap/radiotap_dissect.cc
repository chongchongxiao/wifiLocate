#include "../packet_info.h"
#include <stdint.h>//for uint_16....
#include "../../common/util.h"//for PRINT_DEBUG

/*wireshark0.99.2弃用的信道求法
static int ieee80211_mhz2ieee	(int freq, int flags) {
#define IS_CHAN_IN_PUBLIC_SAFETY_BAND(_c) ((_c) > 4940 && (_c) < 4990)
	if (flags & IEEE80211_CHAN_2GHZ) { // 2GHz band 
		if (freq == 2484)
			return 14;
		if (freq < 2484)
			return (freq - 2407) / 5;
		else
			return 15 + ((freq - 2512) / 20);
	} else if (flags & IEEE80211_CHAN_5GHZ) { // 5Ghz band 
		if (IS_CHAN_IN_PUBLIC_SAFETY_BAND(freq))
			return ((freq * 10) + (((freq % 5) == 2) ? 5 : 0) - 49400) / 5;
		if (freq <= 5000)
			return (freq - 4000) / 5;
		else
			return (freq - 5000) / 5;
	} else { // either, guess 
		if (freq == 2484)
			return 14;
		if (freq < 2484)
			return (freq - 2407) / 5;
		if (freq < 5000) {
			if (IS_CHAN_IN_PUBLIC_SAFETY_BAND(freq))
				return ((freq * 10) + (((freq % 5) == 2) ? 5 : 0) - 49400) / 5;
			else if (freq > 4900)
				return (freq - 4000) / 5;
			else
				return 15 + ((freq - 2512) / 20);
		}
		return (freq - 5000) / 5;
	}
#undef IS_CHAN_IN_PUBLIC_SAFETY_BAND
}
*/



/********************************************************/
/*	频率与信道的转换 				*/
/*	参考wireshark1.11.2/epan/frequency-utils.c	*/
/* 	添加日期：2016.01.08				*/
/********************************************************/

//信道集合结构体定义
typedef struct freq_cvt_s {
    uint16_t fmin;  /*最小频率，MHz */
    uint16_t fmax;  /*最大频率， MHz */
    int cmin;       /*最小信道号 */
    bool is_bg;     /* 是否为B/G信道*/
} freq_cvt_t;

#define FREQ_STEP 5 /* MHz.信道间隔频率固定为5MHz*/
#define FREQ_IS_BG(freq) (freq <= 2484)
/*信道集合数组,引用自"802.11 Wireless Networks: The Definitive Guide", 2nd Ed. by Matthew Gast */
static freq_cvt_t freq_cvt[] = {
    { 2412, 2472,   1, true },   /* Table 12-1, p 257 */
    { 2484, 2484,  14, true },   /* Table 12-1, p 257 */
    { 5000, 5995,   0, false },  /* Table 13-1, p 289 */
    { 4920, 4995, 240, false }   /* Table 13-1, p 289 */
};

#define NUM_FREQ_CVT (sizeof(freq_cvt) / sizeof(freq_cvt_t))//信道集合数（4，上面数组的大小）
#define MAX_CHANNEL(fc) ((int)(fc.fmax - fc.fmin) / FREQ_STEP) + fc.cmin )//信道集合中最大的信道号

/*
 * 给定一个频率，返回一个信道号,未找到返回-1
 */
int ieee80211_mhz_to_chan(uint16_t freq) {
    int i;
    for (i = 0; i < NUM_FREQ_CVT; i++) {
        if (freq >= freq_cvt[i].fmin && freq <= freq_cvt[i].fmax) {
            return ((freq - freq_cvt[i].fmin) / FREQ_STEP) + freq_cvt[i].cmin;
        }
    }
    return -1;
}
/*
给定一个频率返回信道的字符串表示,备用!!
*/
/*
string ieee80211_mhz_to_str(unsigned int freq){
    int chan = ieee80211_mhz_to_chan(freq);
    bool is_bg = FREQ_IS_BG(freq);
    if (chan < 0) {
        return g_strdup_printf("%u", freq);
    } else {
        return g_strdup_printf("%u [%s %u]", freq, is_bg ? "BG" : "A",
            chan);
    }
}
*/




//radiotapHeader定义结构体
struct ieee80211_radiotap_header {
	u_char it_version;	//版本号当前只有0
	u_char it_pad;		//预留空白位，未使用
	uint16_t it_len;        //整个头部长度
#define MAX_PRESENT 1
	uint32_t it_present[MAX_PRESENT];        //信息标识位，可能有多个，此处只处理一个
};

//存在的信息类型标识枚举
enum ieee80211_radiotap_type {
	IEEE80211_RADIOTAP_TSFT = 0,
	IEEE80211_RADIOTAP_FLAGS = 1,
	IEEE80211_RADIOTAP_RATE = 2,
	IEEE80211_RADIOTAP_CHANNEL = 3,
	IEEE80211_RADIOTAP_FHSS = 4,
	IEEE80211_RADIOTAP_DBM_ANTSIGNAL = 5,
	IEEE80211_RADIOTAP_DBM_ANTNOISE = 6,
	IEEE80211_RADIOTAP_LOCK_QUALITY = 7,
	IEEE80211_RADIOTAP_TX_ATTENUATION = 8,
	IEEE80211_RADIOTAP_DB_TX_ATTENUATION = 9,
	IEEE80211_RADIOTAP_DBM_TX_POWER = 10,
	IEEE80211_RADIOTAP_ANTENNA = 11,
	IEEE80211_RADIOTAP_DB_ANTSIGNAL = 12,
	IEEE80211_RADIOTAP_DB_ANTNOISE = 13,
	IEEE80211_RADIOTAP_EXT = 31
};

//定义flags域中的相关标志掩码
#define	IEEE80211_RADIOTAP_F_DATAPAD	0x20	//ieee80211帧首部与数据间是否有padding，对其到32位边界
#define IEEE80211_RADIOTAP_F_WEP	0X04	// ap是否采用wep加密方式

						 
//找到32位标志位的第一个非0位
#define BITNO_32(x) (((x) >> 16) ? 16 + BITNO_16((x) >> 16) : BITNO_16((x)))//若位数大于16，则从高16位找BITNO_16(x)，否则在低16位找BITNO_16(x)
#define BITNO_16(x) (((x) >> 8) ? 8 + BITNO_8((x) >> 8) : BITNO_8((x)))//若位数大于8，则从高八位找BITNO_8(x)，否则在低八位找BITNO_8(x)
#define BITNO_8(x) (((x) >> 4) ? 4 + BITNO_4((x) >> 4) : BITNO_4((x)))//若位数大于4，则从高四位找BITNO_4(x)，否则在低四位找BITNO_4(x)
#define BITNO_4(x) (((x) >> 2) ? 2 + BITNO_2((x) >> 2) : BITNO_2((x)))//若位数大于2，那么取第4位+2，否则取第2位
#define BITNO_2(x) (((x) & 2) ? 1 : 0)//取第二位的二进制值
#define BIT(n)	(1 << n)


//自定义函数，获取radiotap帧数据开始位置
int get_radiotap_data_offset(const u_char* data,int length){
	if(length<8)
	  return -1;
	int offset=7;
	while(offset<length && ((data[offset]&0x80)==0x80)){
		offset+=4;
	}
	if(offset>=length)
		return -1;
	return offset+1;
}

bool dissect_radiotap(const u_char* data, Packet_info* pkt_info) {
	int offset = 0; //处理到的字符偏移量（初始为0）
	const struct ieee80211_radiotap_header *hdr = (const struct ieee80211_radiotap_header *) data;//直接用结构体指针将比特流读取成radiotap帧头
	int length, length_remaining; //长度及未处理长度
	uint8_t rate=0;//速率
	uint16_t freq, flags; //速率/频率及标识
	freq = flags = 0;
	int8_t dbm = 0; //信号强度
	uint8_t db,rflags; //噪音及标识
	db = rflags = 0;
	uint32_t present, next_present; //当前信息块和下一个信息块

	length = hdr->it_len; //第3、4位为长度
	present = hdr->it_present[0]; //后面32位为present

	length_remaining = length; //当前剩余长度为总长度

	//仅当标志位只有32位时有效，如果超出无法处理
	//当长度不够时表明帧有问题，直接返回
	if (length_remaining < sizeof(struct ieee80211_radiotap_header)) {
		PRINT_DEBUG("radiotap_dissect.cc:错误 报文长度不合法");
		return false;
	}

	//处理游标后移
	offset += get_radiotap_data_offset(data,length);
	if(offset<0)
	{
		PRINT_DEBUG("radiotap_dissect.cc:错误 获取数据起点失败");
		return false;
	}
	PRINT_DEBUG("radiotap_dissect.cc:信息 数据起点(未对齐)：" << offset );
	length_remaining =length-offset;
	for (; present; present = next_present) {
		/* clear the least significant bit that is set */
		next_present = present & (present - 1);    //清除第一个非零位的方法

		/* extract the least significant bit that is set */
		int bit = BITNO_32(present ^ next_present);    //得到第一个非零位
		switch (bit) {
		case IEEE80211_RADIOTAP_TSFT://0 u64
			//寻找对齐点(8字节对齐)
			while(offset%8!=0)
			{
				offset++;
				length_remaining--;
			}
			if (length_remaining < 8)
				break;
			offset += 8;
			length_remaining -= 8;
			break;
		case IEEE80211_RADIOTAP_FLAGS://1 u8    
			if (length_remaining < 1)
				break;
			rflags = data[offset];
			offset++;
			length_remaining--;
			break;
		case IEEE80211_RADIOTAP_RATE://2 u8 速率
			if (length_remaining < 1)
				break;
			rate = data[offset] & 0x7f;
			//注意速率的表示形式："Data Rate: %d.%d Mb/s", rate / 2, rate & 1 ? 5 : 0最后一位为小数位
			offset++;
			length_remaining--;
			break;
		case IEEE80211_RADIOTAP_CHANNEL://3 u16 freq u16 flags 信道
			//寻找对齐点(2字节对齐)
			while(offset%2!=0)
			{
				offset++;
				length_remaining--;
			}
			if (length_remaining < 4)
				break;
			/*
			if (length > 18) {
				offset++;
			}*/
			freq = *((uint16_t*)(data+offset));
			flags = *((uint16_t*)(data+offset+2));
			offset += 4;
			length_remaining -= 4;
			break;
			
		case IEEE80211_RADIOTAP_DBM_ANTSIGNAL://5 s8 信号强度
			if (length_remaining < 1)
				break;
			dbm = data[offset];    //因为有两处所以放在最后再保存
			//表示："SSI Signal: %d dBm
			offset++;
			length_remaining--;
			break;
		case IEEE80211_RADIOTAP_DBM_ANTNOISE://6 s8 噪音强度
			if (length_remaining < 1)
				break;
			dbm = data[offset];
			//表示："SSI Noise: %d dBm", dbm
			offset++;
			length_remaining--;
			break;
		case IEEE80211_RADIOTAP_DBM_TX_POWER://10 s8 电源
			if (length_remaining < 1)
				break;
			offset++;
			length_remaining--;
			break;
		case IEEE80211_RADIOTAP_ANTENNA://11 u8 是否有天线
			if (length_remaining < 1)
				break;
			offset++;
			length_remaining--;
			break;
		case IEEE80211_RADIOTAP_DB_ANTSIGNAL://12 u8 天线信号
			if (length_remaining < 1)
				break;
			db = data[offset];
			//表示："SSI Signal: %u dB", db
			offset++;
			length_remaining--;
			break;
		case IEEE80211_RADIOTAP_DB_ANTNOISE://13 u8 天线噪音====
			if (length_remaining < 1)
				break;
			db = data[offset];
			//表示："SSI Noise: %u dB", db
			offset++;
			length_remaining--;
			break;
		case IEEE80211_RADIOTAP_FHSS://4 u8 hop set, u8 hop pattern
		case IEEE80211_RADIOTAP_LOCK_QUALITY://7 u16
		case IEEE80211_RADIOTAP_TX_ATTENUATION://8 u16
		case IEEE80211_RADIOTAP_DB_TX_ATTENUATION://9 u16
			//寻找对齐点(2字节对齐)
			while(offset%2!=0)
			{
				offset++;
				length_remaining--;
			}
			if (length_remaining < 2)
				break;
			offset += 2;
			length_remaining -= 2;
			break;
		default:
			/*
			 * This indicates a field whose size we do not
			 * know, so we cannot proceed.
			 */
			next_present = 0;
			continue;
		}
	}

	//解析完成后将结果保存到packet_info中 
	pkt_info->rthdr_len = length;
    PRINT_DEBUG("radiotap_dissect.cc:信息 radiotap头部长度："<< pkt_info->rthdr_len);

	pkt_info->data_rate = (rate / 2 + ((rate & 1) ? 0.5 : 0.0));
    PRINT_DEBUG("radiotap_dissect.cc:信息 速率："<< pkt_info->data_rate);

	pkt_info->rssi_signal = dbm;	 
    PRINT_DEBUG("radiotap_dissect.cc:信息 信号强度："<< (int)dbm );
	
	//pkt_info->channel_id = ieee80211_mhz2ieee(freq, flags);//wireshark0.99.2方式（弃用） 
	pkt_info->channel_id=ieee80211_mhz_to_chan(freq);//参考wireshark1.11.2源码后获取信道的方式
    PRINT_DEBUG("radiotap_dissect.cc:信息 信道："<< pkt_info->channel_id );

	pkt_info->datapad = (rflags & IEEE80211_RADIOTAP_F_DATAPAD) ? true : false;	//是否需要对齐，解析ieee80211需要******** 
	pkt_info->wep_encrypted=(rflags & IEEE80211_RADIOTAP_F_WEP) ? true : false;	//是否采用wep加密方式
    PRINT_DEBUG("radiotap_dissect.cc:信息 IEEE80211是否留白："<< ((pkt_info->datapad)?"true":"false"));
	return true;
}



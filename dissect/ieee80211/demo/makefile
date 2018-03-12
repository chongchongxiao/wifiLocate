#功能：创建测试ieee80211_dissect的程序
#创建日期：2015.12.02
#创建人：mushao
MAIN_DIR=../../..
include $(MAIN_DIR)/common.mak
#要生成的测试程序
PRG=$(IEEE80211_DISSECT_DIR)/demo/ieee80211_dissect_demo
#中间文件
OBJ_IEEE80211=$(IEEE80211_DISSECT_DIR)/ieee80211_dissect.o\
	      $(IEEE80211_DISSECT_DIR)/demo/ieee80211_dissect_demo.o\
	      $(COM_DIR)/util.o
#生成规则
$(PRG):$(OBJ_IEEE80211)
	$(CC) -o $@ $(OBJ_IEEE80211)
	@echo "生成demo完成"
#demo生成规则单独给出
$(IEEE80211_DISSECT_DIR)/demo/ieee80211_dissect_demo.o:$(IEEE80211_DISSECT_DIR)/demo/ieee80211_dissect_demo.cc $(DISSECT_DIR)/packet_info.h
	$(CC) -c $< -o $@ $(CPPFLAGS)
#引入目标文件生成规则
include $(MAIN_DIR)/rules.mak
#清除规则
.PHONY:clean
clean:
	rm -f $(PRG) $(OBJ_IEEE80211)
	@echo "清除编译文件完成"

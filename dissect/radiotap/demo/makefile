#功能：生成测试radiotap_dissect的程序
#创建日期：2015.12.02
#创建人：mushao
MAIN_DIR=../../..
include $(MAIN_DIR)/common.mak
#要生成的测试程序
PRG_RADIOTAP=radiotap_dissect_demo
OBJ_RADIOTAP=$(RADIOTAP_DISSECT_DIR)/radiotap_dissect.o $(COM_DIR)/util.o $(RADIOTAP_DISSECT_DIR)/demo/radiotap_dissect_demo.o 
$(PRG_RADIOTAP):$(OBJ_RADIOTAP)
	$(CC) -o $@ $(OBJ_RADIOTAP)
	@echo "生成demo完成"
#测试文件要单独给出生成规则
$(RADIOTAP_DISSECT_DIR)/demo/radiotap_dissect_demo.o:$(RADIOTAP_DISSECT_DIR)/demo/radiotap_dissect_demo.cc $(DISSECT_DIR)/packet_info.h
	$(CC) -c $< -o $@ $(CPPFLAGS)
#引入目标文件生成规则
include $(MAIN_DIR)/rules.mak
.PHONY:clean
clean:
	rm -f $(PRG_RADIOTAP) $(OBJ_RADIOTAP)
	@echo "清除编译文件完成"

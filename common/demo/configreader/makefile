#功能：创建测试configreader的程序
#创建日期：2016.01.29
#创建人：mushao

MAIN_DIR=../../..
include $(MAIN_DIR)/common.mak
#要生成的测试程序
PRG_CONFIGREADER=$(COM_DIR)/demo/configreader/configreader_demo
#生成的中间目标文件
OBJ_CONFIGREADER:=$(COM_DIR)/configreader.o $(COM_DIR)/logwriter.o $(COM_DIR)/util.o $(COM_DIR)/confighandler.o $(COM_DIR)/demo/configreader/configreader_demo.o
$(PRG_CONFIGREADER):$(OBJ_CONFIGREADER)
	$(CC) -o $@ $(OBJ_CONFIGREADER)
	@echo "生成demo完成"
#引入目标生成规则
include $(MAIN_DIR)/rules.mak
#demo目标文件生成规则单独给出
$(COM_DIR)/demo/configreader/configreader_demo.o:$(COM_DIR)/demo/configreader/configreader_demo.cc $(COM_DIR)/configreader.h
	$(CC) -c $< -o $@ $(CPPFLAGS)

.PHONY:clean
clean:
	rm -f $(PRG_CONFIGREADER) $(OBJ_CONFIGREADER) bssidinfo.conf setting.conf
	rm -fr config log
	@echo "清除编译文件完成"

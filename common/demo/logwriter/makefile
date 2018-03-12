#功能：创建测试logwriter的程序
#创建日期：2016.01.28
#创建人：mushao

MAIN_DIR=../../..
include $(MAIN_DIR)/common.mak
#要生成的测试程序
PRG_LOGWRITER=$(COM_DIR)/demo/logwriter/logwriter_demo
#生成的中间目标文件
OBJ_LOGWRITER:=$(COM_DIR)/logwriter.o $(COM_DIR)/util.o $(COM_DIR)/demo/logwriter/logwriter_demo.o
$(PRG_LOGWRITER):$(OBJ_LOGWRITER)
	$(CC) -o $@ $(OBJ_LOGWRITER)
	@echo "生成demo完成"

#引入目标文件生成规则
include $(MAIN_DIR)/rules.mak
#demo目标文件无特殊依赖项，采用隐式规则

.PHONY:clean
clean:
	rm -f $(PRG_LOGWRITER) $(OBJ_LOGWRITER) 
	rm -fr config log
	@echo "清除编译文件完成"

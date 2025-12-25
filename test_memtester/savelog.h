/*
 * savelog.h
 *
 *  Created on: 2016-6-28
 *
 */

#ifndef SAVELOG_H_
#define SAVELOG_H_

#include <assert.h>
#include <stdio.h>
#include <sys/select.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/dispatch.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

#define LOGFILEPATH1 "/storage/log1.txt"
#define LOGFILEPATH2 "/storage/log2.txt"
#define LOGFILESIZE  3000000

#define DEBUG 0   //DEBUG为1，在终端打印信息。为0时只在文件中记录

void slogf_file(const char* fmt,...);//按照printf格式记录到文件中
void slogf_time(void);//记录当前时间到文件中
#endif /* SAVELOG_H_ */

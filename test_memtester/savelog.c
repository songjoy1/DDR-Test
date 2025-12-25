/*
 * savelog.c
 *
 *
 */

#include "savelog.h"
/*
    功能：    打开文件，根据文件大小和修改时间确定当前操作的文件，
    返回值：    写文件成功返回文件指针,否则返回NULL
*/

static FILE* openFile();
static int writeFile(FILE *fp, const char *str,va_list arglist);
static int closeFile(FILE *fp);

static FILE* openFile()  // 打开文本文件
{
	FILE *fp = NULL;
	struct stat statbuff1, statbuff2;
	unsigned int filesize1 = 0, filesize2 = 0;
	unsigned char open_faultfile_gul = 1;//表明当前操作的文件：1—log1;2-log2
	if (stat(LOGFILEPATH1, &statbuff1) != -1)
		filesize1 = statbuff1.st_size;
	else
		filesize1 = 0;
	if (stat(LOGFILEPATH2, &statbuff2) != -1)
		filesize2 = statbuff2.st_size;
	else
		filesize2 = 0;
	if ((filesize1 < LOGFILESIZE) && (filesize2 < LOGFILESIZE)) {
		open_faultfile_gul = 1;
	}else if ((filesize1 < LOGFILESIZE) && (filesize2 > LOGFILESIZE)) {
		open_faultfile_gul = 1;
	}else if ((filesize1 > LOGFILESIZE) && (filesize2
			< LOGFILESIZE)) {
		open_faultfile_gul = 2;
	} else if ((filesize1 >= LOGFILESIZE) && (filesize2 >= LOGFILESIZE)) {
		if (statbuff1.st_mtime < statbuff2.st_mtime) {
			open_faultfile_gul = 1;
			remove(LOGFILEPATH1);
		} else{
			open_faultfile_gul = 2;
			remove(LOGFILEPATH2);
		}
	}
	if (open_faultfile_gul == 1) {
		fp = fopen(LOGFILEPATH1, "a+");
		if (NULL == fp) {
			remove(LOGFILEPATH1);
		}
	} else if (open_faultfile_gul == 2) {
		fp = fopen(LOGFILEPATH2, "a+");
		if (NULL == fp) {
			remove(LOGFILEPATH2);
		}
	}
    return fp;
}

/*
    功能：     将str写入到文件
    返回值：    写文件成功返回0,否则返回-1
    fp:     文件指针
    str:    待写入的字符串
    bLog:   预留标志日志文件故障等级，默认为0
    说明：     如果是日志文件，将会在str前加上当前时间(格式如：2011-04-12 12:10:20)
*/
static int writeFile(FILE *fp, const char *format,va_list arglist)
{
	int ret = -1;
#if DEBUG
	vfprintf(stderr, format, arglist);
#endif
	ret=vfprintf(fp, format, arglist);
	if (ret >= 0) {
		fflush(fp);
		return 0; // 写文件成功
	} else
		return -1;
}

/*
 * 记录时间*/
static void writeTime(FILE *fp)
{
	time_t t;
	struct tm *tp;
	t = time(NULL);
	tp = localtime(&t);
	char curTime[100] = { 0 };

	sprintf(curTime, "%2.2d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d", tp->tm_year + 1900,
				tp->tm_mon + 1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
	fprintf(fp, "[%s]\r\n",curTime);
}

/*
 * 关闭文件接口
 */
static int closeFile(FILE *fp) {
	if(fp != NULL)
		return fclose(fp);
	return 0;
}



void record_event_log(int timeRecord, const char* fmt,...) {
	va_list arglist;
	FILE *re_fd;
	re_fd = openFile();
	if(re_fd!=NULL){
		if(timeRecord)
			writeTime(re_fd);
		va_start(arglist, fmt);
		writeFile(re_fd, fmt,arglist);
		va_end(arglist);
		closeFile(re_fd);
	}else{
		printf("openFile error %d\n", errno);
	}
}

void slogf_file(const char* fmt,...) {
	va_list arglist;
	FILE *re_fd;
	re_fd = openFile();
	if(re_fd!=NULL){
		va_start(arglist, fmt);
		writeFile(re_fd, fmt,arglist);
		va_end(arglist);
		closeFile(re_fd);
	}else{
		printf("openFile error %d\n", errno);
	}
}

void slogf_time(void)
{
	FILE *re_fd;
    re_fd = openFile();
	if (re_fd != NULL) {
		writeTime(re_fd);
		closeFile(re_fd);
	} else {
		printf("openFile error %d\n", errno);
	}
}

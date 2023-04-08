#ifndef PACKET_H
#define PACKET_H
#define SIZE_BUFFER_RECV 4*1024 

#define SIZE_START_PK 2 + 1

#define SIZE_ALLOWING_PK 2 + 1

#define SIZE_BUFFER_FILE 4 * 1024 - 4 - 2 - 1
#define SIZE_FILE_PK 4 * 1024 

#define SIZE_FILEINFO_PK 200 + 8 + 2 + 1 

#define SIZE_BUFFER_MESSAGE 1 * 1024 - 2 - 2 -1
#define SIZE_MESSAGE_PK 1 * 1024

#define SIZE_SYSTEM_MESSAGE_PK  2 + 2 + 1


enum HEADER : char
{
	START,
	ALLOWING,
	FILE_BIG,
	FILEINFO,
	MESSAGE,
	SYSTEM_MESSAGE,
};
struct packet
{
	HEADER header;
};
struct start : public packet
{
	unsigned short int allowingID;
	start()
	{
		header = HEADER::START;
	}
};
struct allowig : public packet
{
	unsigned short int allowingID;
	allowig()
	{
		header = HEADER::ALLOWING;
	}
};

struct file : public packet
{
	unsigned short int allowingID;
	char buffer[SIZE_BUFFER_FILE];
	unsigned long int  Number;
	file()
	{
		header = HEADER::FILE_BIG;
		memset(buffer, 0, SIZE_BUFFER_FILE);

	}
};
struct fileInfo : public packet
{
	unsigned short int allowingID;
	char name[200];
	unsigned long long int sizeAll;
	fileInfo()
	{
		memset(name, 0, 200);
		header = HEADER::FILEINFO;
	}
};
struct message : public packet
{
	unsigned short int allowingID;
	char buffer[SIZE_BUFFER_MESSAGE];
	unsigned short int size;
	message()
	{
		memset(buffer, 0, SIZE_BUFFER_MESSAGE);
		header = HEADER::MESSAGE;
	}
};
struct system_message : public packet
{
	unsigned short int allowingID;
	unsigned short int Code;
	system_message()
	{

		header = HEADER::MESSAGE;
	}
};
#endif


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>
int main()
{
int sockfd; /* số mô tả socket – socket handle */
int len;
struct sockaddr_un address; /* structure quan trọng, chứa các thông tin về socket */
struct mess {
int message_type;
int UE_ID;
int TAC;
int CN_Domain;
}NGAP;
int result;
int temp; // bien tam
while(1){//*
sleep(3);
NGAP.message_type = 99+Get_Random(0,1);
NGAP.UE_ID=Get_Random(2,4);
NGAP.TAC =100;
NGAP.CN_Domain=101;
/* 2. Tạo socket cho trình khách. Lưu lại số mô tả socket */
sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
address.sun_family = AF_UNIX;
/* 3. Gán tên của socket trên máy chủ cần kết nối */
strcpy( address.sun_path, "server_socket" );
len = sizeof( address );
/* 4. Thực hiện kết nối */
result = connect( sockfd, (struct sockaddr*)&address, len );
if ( result == -1 ) {
perror( "Oops: client1 problem" );
exit( 1 );
}
/* 5. Sau khi socket kết nối, chúng ta có thể đọc ghi dữ liệu của socket tương tự đọc ghi
trên file */
temp = NGAP.message_type;
write (sockfd, &temp,1);
temp = NGAP.UE_ID;
write (sockfd, &temp,1);
temp =NGAP.TAC;
write (sockfd, &temp,1);
temp =NGAP.CN_Domain;
write (sockfd, &temp,1);
read ( sockfd, &temp, 1 );
//printf("%d\n",temp);
if (temp == 1){
printf("Ban tin khong hop le\n");
}
if(temp == 0){
printf("Truyen thanh cong\n");
}
close( sockfd );
//exit( 0 );
}//*
}
int Get_Random(int min,int max){
 return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}
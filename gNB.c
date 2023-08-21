#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>
int main()
{
time_t begin = time(NULL);
int server_sockfd, client_sockfd;
int server_len, client_len;
struct sockaddr_un server_address;
struct sockaddr_un client_address;
struct mess {
int message_type;
int UE_ID;
int TAC;
int CN_Domain;
}NGAP;
int temp=0;
int temp1;
int ue_id_i = 3;
int Dong_bo = 1;
int chu_ky = 1;
int i = 0;
int SFN_gNB=0;
struct mess Queue[20];
/* 2. Loại bỏ các tên hay liên kết socket khác trước đó nếu có. Đồng thời thực hiện khởi
tạo socket mới cho trình chủ */
unlink( "server_socket" );
server_sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
/* 3. Đặt tên cho socket của trình chủ */
server_address.sun_family = AF_UNIX;
strcpy( server_address.sun_path, "server_socket" );
server_len = sizeof( server_address );
/* 4. Ràng buộc tên với socket */
bind( server_sockfd, (struct sockaddr *)&server_address, server_len );
/* 5. Mở hàng đợi nhận kết nối - cho phép đặt hàng vào hàng đợi tối đa 5 kết nối */
listen( server_sockfd, 5 );
signal (SIGCHLD, SIG_IGN);
/* 6. Lặp vĩnh viễn để chờ và xử lý kết nối của trình khách */
//fork();
while ( 1 ) {
printf( "server waiting...\n" );
/* Chờ và chấp nhận kết nối */
client_sockfd = accept( server_sockfd, (struct sockaddr*)&client_address, &client_len );
time_t end = time(NULL);
SFN_gNB = end - begin;
if (SFN_gNB > 1024){
SFN_gNB = SFN_gNB - 1024;
}
printf("The elapsed time is %d seconds\n", SFN_gNB);
/* Đọc dữ liệu do trình khách gửi đến */
read(client_sockfd, &temp,1);
temp1 = temp;
if(temp1 == 1&&Dong_bo==1){
printf("Dong_bo: %d\n", temp1);
write(client_sockfd, &SFN_gNB,1);
read(client_sockfd, &ue_id_i,1);
read(client_sockfd, &chu_ky,1);
printf("chu_ky: %d\n", chu_ky);
printf("ue_id_i: %d\n", ue_id_i);
Dong_bo = 0;
}
else if(temp1!=0){//1
NGAP.message_type=temp;
printf("%d\n", temp);
read(client_sockfd, &temp,1);
NGAP.UE_ID=temp;
printf("%d\n", temp);
read(client_sockfd, &temp,1);
NGAP.TAC=temp;
printf("%d\n", temp);
read(client_sockfd, &temp,1);
NGAP.CN_Domain=temp;
printf("%d\n", temp);
if (NGAP.message_type == 100){
if(NGAP.TAC ==100){
if(NGAP.CN_Domain == 100 || NGAP.CN_Domain == 101){
temp = 0;
write( client_sockfd, &temp,1 );
Queue[i] =NGAP;
i = i+1;
printf("%d\n", i);
}
else{
temp = 1;
write(client_sockfd, &temp,1 );
}
}
else{
temp = 1;
write(client_sockfd, &temp,1 );
}
}
else{
temp = 1;
write(client_sockfd, &temp,1 );
}
}//1
else{
if(((SFN_gNB-ue_id_i)%chu_ky)==0 && Dong_bo == 0){
temp = i;
write(client_sockfd, &temp,1);
while ( i > 0){
temp = Queue[i-1].message_type;
write(client_sockfd, &temp,1);
temp = Queue[i-1].UE_ID;
write(client_sockfd, &temp,1);
temp =Queue[i-1].TAC;
write(client_sockfd, &temp,1);
temp =Queue[i-1].CN_Domain;
write(client_sockfd, &temp,1);
i = i -1 ;
printf("%d\n",i);
}
}
}
/* Đóng kết nối */
close( client_sockfd );
}
}
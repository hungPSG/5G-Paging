#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
struct mess {
int message_type;
int UE_ID;
int TAC;
int CN_Domain;
}RRC;
int ue_id_i =3;
int chu_ky = 17;
int SFN_UE = 0;
int Dong_bo = 1;
int temp =0;
int k = 0;
int i = 0;
while(1){
printf("SFN_UE: %d\n", SFN_UE);
if(SFN_UE > 1024){
SFN_UE = SFN_UE - 1024;
k = 0;
}
int sockfd=0; /* số mô tả socket – socket handle */
int len=0;
struct sockaddr_un address; /* structure quan trọng, chứa các thông tin về socket */
int result=0;
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
/*5. Sau khi socket kết nối, chúng ta có thể đọc ghi dữ liệu của socket tương tự đọc ghi
trên file */
if(Dong_bo == 1){ // qua trinh dong bo thoi gian cua UE
write (sockfd, &Dong_bo,1);
printf("Dong Bo: %d\n", Dong_bo);
read ( sockfd, &SFN_UE, 1 );
printf("SFN_UE: %d\n", SFN_UE);
write (sockfd, &ue_id_i,1);
write (sockfd, &chu_ky,1);
k = (int)((SFN_UE-ue_id_i)/chu_ky);
printf("K: %d\n",k);
temp = ue_id_i + (k+1)*chu_ky - SFN_UE; // tinh khoang thoi gian can phai
sleep de dong bo
printf("temp: %d\n", temp);
close( sockfd );
sleep(temp);
SFN_UE = ue_id_i + (k+1)*chu_ky;
printf("SFN_UE: %d\n", SFN_UE);
}
else{
write (sockfd, &Dong_bo,1);
read(sockfd, &temp,1);
i = temp;
printf("i: %d\n",i);
while ( i > 0){
read(sockfd, &temp,1);
RRC.message_type=temp;
printf("RRC.message: %d\n", temp);
read(sockfd, &temp,1);
RRC.UE_ID=temp;
printf("RRC.UE_ID: %d\n", temp);
read(sockfd, &temp,1);
RRC.TAC=temp;
//printf("%d\n", temp);
read(sockfd, &temp,1);
RRC.CN_Domain=temp;
//printf("%d\n", temp);
printf("SFN_UE: %d\n", SFN_UE);
i = i -1;
if (RRC.message_type == 100 && RRC.UE_ID == ue_id_i){
printf("Paging Thanh Cong\n");
}
else {
printf("Ban Tin Cua UE khac\n");
}
}
sleep(chu_ky);
SFN_UE=SFN_UE+chu_ky;
close( sockfd );
}
Dong_bo = 0;
}
}
int Get_Random(int min,int max){
 return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}
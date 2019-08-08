#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024 // buff size  
#define RLT_SIZE 4 //피연산자의 바이트 수
#define OPSZ 4  //연산자으 ㅣ바이트 수
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char opmsg[BUF_SIZE];   //메모리 공간을 생성한다
	int result, opnd_cnt, i;
	struct sockaddr_in serv_adr;
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");

	fputs("Operand count: ", stdout);
	scanf("%d", &opnd_cnt); //피연산자의 개수 정보를 입력받는다 
	opmsg[0]=(char)opnd_cnt;
	
	for(i=0; i<opnd_cnt; i++)   //반복문을 이용해서 정수를 연속으로 배열에 저장하고 있다
	{
		printf("Operand %d: ", i+1);
		scanf("%d", (int*)&opmsg[i*OPSZ+1]);
	}
	fgetc(stdin); //버퍼의 '\n'문자를 삭제한다
	fputs("Operator: ", stdout);
	scanf("%c", &opmsg[opnd_cnt*OPSZ+1]); //연산자 정보를 마지막으로 입력 받는다
	write(sock, opmsg, opnd_cnt*OPSZ+2);    //wirte 함수 호출을 통해 연산과 관련된 정보를 전송하고 있다->TCP는 데이터의 경계가 존재하지 않으므로 한번에 보내도되고, 나누어 보내도 된다
	read(sock, &result, RLT_SIZE);  //정보를 받는다
	
	printf("Operation result: %d \n", result);
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
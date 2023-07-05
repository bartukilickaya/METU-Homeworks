// server.c
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define MAXLINE 1024 // MAX message size
#define BUFFER_SIZE 262144 // TOTAL BUFFER SIZE THAT CLIENT CAN SEND
#define MESSAGE_SIZE 10 //10 + 1 + 2 unsigned short.
#define PACKET_SIZE 14
#define RECEIVE_BUFFER_SIZE 262144


struct message_packet {
    unsigned short seq_no;
    char message[MESSAGE_SIZE];
    unsigned char packet_type;

};

struct sender_window {
  int window_size;
  int send_base; // oldest waiting ACK
  int next_seq_sum; // usable, not yet send
};

struct receiver_window{
  int window_size;
  int rcv_base; // not received packet
};

// global variables
int i = 0;
char line[MAXLINE];
char ch;
int newlineTotal = 0;
unsigned short seq_no = 0;
int sockfd=0;
int sockfd2=0;
int buffer_index = 0;
int buffer_index_receive = 0;
int last_printed_line = 0;
int length_of_line;
int rv;
int numbytes;
char message[MESSAGE_SIZE];
char s[INET_ADDRSTRLEN];
socklen_t addr_len;
struct addrinfo hints, *servinfo, *p;
struct addrinfo hints2, *servinfo2, *p2;
struct sockaddr_storage their_addr;
struct message_packet packet;
struct message_packet buffer[BUFFER_SIZE]; // containin all messages
struct message_packet buffer_receive[RECEIVE_BUFFER_SIZE];
struct sender_window s_window;
struct receiver_window r_window; // receive window
struct message_packet packet_received;

void set_packet(unsigned short* seq_no,int *buffer_index,struct message_packet*buffer,
  char* message,struct message_packet *packet){
    memset(packet, '\0', PACKET_SIZE);
    (*packet).seq_no = *(seq_no); // set packet's seq_no
    (*packet).packet_type = 0; // 0 for message packets, 1 for ACK
    strncpy((*packet).message,message,MESSAGE_SIZE); // copy line
    memcpy(buffer+ (*buffer_index),packet,PACKET_SIZE); // copy to buffer
    return;
}

void set_sender_window(struct sender_window* s_window){
  (*s_window).window_size = 65535;
  (*s_window).send_base = 0;
  (*s_window).next_seq_sum = 0;
  return;
}

void set_receiver_window(struct receiver_window *r_window){
  (*r_window).window_size = 65535; // 1 byte
  (*r_window).rcv_base = 0;
  return;
}

void set_ACK_packet(unsigned short *seq_no,struct message_packet *packet ){
    memset((*packet).message, '\0', MESSAGE_SIZE);
    (*packet).seq_no = *seq_no;
    (*packet).packet_type = 1; // ACK packet type
    return;
}
// get sockaddr, IPv4
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void *send_packets(void *vargp){
  while(1){
    i = 0;
    memset(line, '\0', MAXLINE); // null line before
    while ( (ch = getchar()) != EOF) { // read input with newline
        if( ch == '\n' && i != 0){
          newlineTotal = 0;
          line[i] = ch;
          i++;
          break;
        }
        else if( ch == '\n' ){
          newlineTotal++;
          line[i] = ch;
          i++;
          break;
        }
        line[i] = ch;
        i++;
    }
    if( newlineTotal >= 2 ){
        // FINISH CONVERSATION
        // SEND TYPE 2 EXIT PACKET
        set_ACK_packet(&seq_no,&packet);
        packet.packet_type = 2;
        for(int j = 0 ; j < 20; j++){
          if ((numbytes = sendto(sockfd2,&packet, PACKET_SIZE, 0,
                                 p2->ai_addr, p2->ai_addrlen)) != -1)
          {

          }
        }
        printf("Connection Terminated by the server.\n" );
        exit(1);
    }
  length_of_line = i;
  // PUT ENTIRE LINE IN BUFFER
  for(int j = 0; j < length_of_line; j+=MESSAGE_SIZE){
    //printf("Putting input to buffer\n" );
    strncpy(message,line+j,MESSAGE_SIZE);
    set_packet(&seq_no,&buffer_index,buffer,message,&packet);
    seq_no++;
    if( s_window.next_seq_sum - s_window.send_base < s_window.window_size ){
      // send packet if next_seq_sum is available
      if ((numbytes = sendto(sockfd2,&buffer[buffer_index], PACKET_SIZE, 0,
                             p2->ai_addr, p2->ai_addrlen)) != -1)
      {
              //printf("Packet SENT!\n" );

      }
      s_window.next_seq_sum++;
    }
    /*printf ("pkg message: %.10s . seqNo: %d , type: %d \n",
    packet.message, packet.seq_no,packet.packet_type);*/
    buffer_index++;

  }
  }
  return NULL;
}

void *receive_packets(void *vargp){
  while (1) {
  // PACKET RECEIVED
  //printf("Waiting at receive:\n" );
  if ((numbytes = recvfrom(sockfd, &packet_received, sizeof(struct message_packet), 0,
                           (struct sockaddr *)&their_addr, &addr_len)) != -1)
  {
      if( packet_received.packet_type == 0 ){ // normal packet (NOT ACK)
        /*printf("server: got packet from %s\n",
        inet_ntop(their_addr.ss_family,
                    get_in_addr((struct sockaddr *)&their_addr),
                    s, sizeof s));*/
        /*printf("server: packet contains: %.10s,seqno: %d , packet_type: %d message_size: %ld\n",
          packet_received.message,packet_received.seq_no,packet_received.packet_type,strlen(packet_received.message));*/
          if( r_window.rcv_base == packet_received.seq_no ){ // in-order packet received
              memcpy(buffer_receive + buffer_index_receive,&packet_received,PACKET_SIZE); // copy in-order packet to buffer
              r_window.rcv_base++;
              buffer_index_receive++; // increase buffer index
              set_ACK_packet(&(packet_received.seq_no),&packet); // ACK packet created
              // SEND ACK PACKET TO CLIENT
              if((numbytes = sendto(sockfd2,&packet, sizeof(struct message_packet), 0,
                                       p2->ai_addr, p2->ai_addrlen)) != -1){
                  //printf("ACK packet send with seq_no %d\n",packet.seq_no );
              }

              if( packet_received.message[strlen(packet_received.message)-1] == '\n'){ // line fully received!
                for(int index = last_printed_line; index < r_window.rcv_base; index++){ // print line
                  printf("%.10s",buffer_receive[index].message);
                  //exit(1)
                }
                last_printed_line = r_window.rcv_base;
                r_window.rcv_base = buffer_index_receive;
              }
          }
          else{ // unorder packet received
            // send rcv_base-1 ACK packet
            if( r_window.rcv_base != 0){ // if packet 0, there is no previously ACKed packet
              unsigned short last_received_ACK = r_window.rcv_base-1;
              set_ACK_packet(&(last_received_ACK),&packet); // ACK packet created

              if((numbytes = sendto(sockfd2,&packet, sizeof(struct message_packet), 0,
                                       p2->ai_addr, p2->ai_addrlen)) != -1){

              }
              //printf("MESSAGE IS IGNORED DUE TO UN-ORDER PACKET,but sent latest received ACK packet\n" );
            }
          }

      }
      else if( packet_received.packet_type == 1 ){ // ACK PACKET RECEİVED type == 1
        // cumilative ACK received, shift window
        // cumilative ACK received, reset timer,set timer with new packet
        if(  packet_received.seq_no >= s_window.send_base){
          // set new base
          //printf("ACK packet received with seq_no %d\n",packet_received.seq_no );
          s_window.send_base = packet_received.seq_no + 1; // set new un-ACK base
          // SET NEW TIMER TODO
        }
        else{
          // Do nothing, ignore duplicate ACK
        }
      }

      else{ // received_packet_type == 2, clean buffer
        printf("Connection Terminated by the client.\n" );
        exit(1);
      }
  }
  }
  return NULL;
}

void setTimeout()
{
    int milliseconds = 500;
    int milliseconds_since = 0;
    int end = 0;
    int previous_send_base = 0;
    while(1){
      previous_send_base = s_window.send_base;
      milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
      end = milliseconds_since + milliseconds;

    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end); // end of 500ms
    if( previous_send_base == s_window.send_base ){
        // it basically means some previous packet timed out or
        // out-order received and discarded.
        // send with go-back-n procedure
      for(int i = s_window.send_base; i < s_window.next_seq_sum &&
         s_window.send_base != s_window.next_seq_sum;i++){
          // if timeout occurs and
          // send base < nex_seq_sum
        if ((numbytes = sendto(sockfd2,&buffer[i], PACKET_SIZE, 0,
                                p2->ai_addr, p2->ai_addrlen)) != -1)
        {
                  //printf("Go Back-N, unACKed packets being sent\n" );

        }
      }
    }
    /*if( count == 8 ){
      count = 0;
      if( previous_send_base == s_window.send_base &&
        previous_rcv_base == r_window.rcv_base &&
        previous_send_base == buffer_index &&
        previous_rcv_base == buffer_index_receive){
          // clear buffer, nothing is happening
        buffer_index = 0;
        buffer_index_receive = 0;
        seq_no = 0;
        last_printed_line = 0;
        s_window.send_base = 0;
        s_window.next_seq_sum = 0;
        r_window.rcv_base = 0;
      }
    }
    count++; */
  }
}


int main(int argc, char *argv[])
{
    pthread_t send_thread;
    pthread_t receive_thread;
    set_sender_window(&s_window);
    set_receiver_window(&r_window); // set r_window
    memset(line, '\0', MAXLINE); // null line before
    memset(buffer, '\0', BUFFER_SIZE);
    memset(buffer_receive, '\0', BUFFER_SIZE);
    if (argc != 3)
    {
        fprintf(stderr, "usage: ./server <client_ip> <server_port><\n");
        exit(1);
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // set to AF_INET to use IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP
    hints.ai_flags = AI_PASSIVE; // use my IP
    memset(&hints2, 0, sizeof hints2);
    hints2.ai_family = AF_INET; // set to AF_INET to use IPv4
    hints2.ai_socktype = SOCK_DGRAM; // UDP
    hints2.ai_flags = AI_NUMERICHOST;
    if ((rv = getaddrinfo(NULL, argv[2], &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    getaddrinfo(argv[1], argv[2], &hints2, &servinfo2);
    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }

    for (p2 = servinfo2; p2 != NULL; p2 = p2->ai_next)
    {
        if ((sockfd2 = socket(p2->ai_family, p2->ai_socktype,
                             p2->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }
        break;
    }
    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind socket\n");
        return 2;
    }
    if (p2 == NULL)
    {
        fprintf(stderr, "server: failed to create socket\n");
        return 2;
    }
    printf("Server is up!\n");
    addr_len = sizeof their_addr;
    pthread_create(&send_thread, NULL, send_packets, NULL);
    pthread_create(&receive_thread, NULL, receive_packets, NULL);
    // start timer
    setTimeout();
    // start threads
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);


    freeaddrinfo(servinfo); // free address
    freeaddrinfo(servinfo2); // free address
    close(sockfd); //close socket
    close(sockfd2); //close socket

    return 0;
}

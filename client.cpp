#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>
#include <stdio.h>
#include <regex>
#include <iostream>

#define PORT 3000
#define COMMNANDLINE_BUFFER_SIZE 200
#define RESPONSE_LINE_BUFFER_SIZE 150

using namespace std;

int port;

typedef enum
{
  RUTE = 0,
  SOSIRI,
  PLECARI,
  INTARZIERE,
  ESTIMARE,
  STATIE,
  QUIT,
  HELP,
  ERROR
} command_type;

struct request
{
  int fd;
  char location[20];
  command_type type;
  int argc;
  char argv[5][COMMNANDLINE_BUFFER_SIZE];
};

char current_location[20];

command_type get_command_type(char command[]);
struct request get_request(char commandline[]);
void send_routes_request(int fd);
void print_response(int fd);

int main(int argc, char *argv[])
{
  int sd;
  struct sockaddr_in server;
  char msg[100];

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("[client] Eroare la socket().\n");
    return errno;
  }
  
  printf("------------Bine ati venit------------\n");
  printf("Introduceti statia dumneavoastra:\n");
  fgets(current_location, 20, stdin);
  current_location[strlen(current_location)-1]='\0';
  printf("Se trimite comanda de tip rute la server pt. a cere datele statiei curente...\n");
  
  server.sin_family = AF_INET;

  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons(PORT);

  if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[client]Eroare la connect().\n");
    return errno;
  }

  send_routes_request(sd);
  print_response(sd);

  while (1)
  {
    char commandline[COMMNANDLINE_BUFFER_SIZE];
    bzero(commandline, COMMNANDLINE_BUFFER_SIZE);

    printf("Introduceti o comanda:\n");
    fgets(commandline, COMMNANDLINE_BUFFER_SIZE, stdin);

    struct request req = get_request(commandline);

    switch (req.type)
    {
    case ERROR:
      printf("Unknown command! Try using \"help\"\n");
      break;
    case HELP:
      printf("---------Comenzi---------\n-rute -> afiseaza toate plecarile si sosirile din statia actuala in ziua respectiva \n-sosiri -> afiseaza toate sosirile in statia actuala in urmatoarea ora, luand in calcul posibilele intarzieri sau veniri in avans \n-plecari -> afiseaza toate plecarile statia actuala in urmatoarea ora, luand in calcul posibilele intarzieri\n-intarziere <nr. tren> <nr. minute> -> raporteaza o intarziere cu un anumit numar de minute fata de ora din program a trenului cu numarul dat\n-estimare <nr. tren> <nr. minute> -> raporteaza o sosire in avans cu un anumit numar de minute fata de ora din program a trenului cu numarul dat in statia actuala a clientului\n-statie <nume statie> -> modifica statia actuala a clientului cu cea introdusa si trimite automat serverului o cere de afisare a plecarile si sosirile din statia actuala in ziua respectiva\n-quit -> inchide conexiunea cu serverul si opreste executia programului\n-help -> afiseaza functionalitatile aplicatiei\n(Parametrii formati din mai multe cuvinte vor fi scrisi intre ghilimele)\n");
      break;
    case STATIE:
      if (req.argc < 1)
      {
        printf("Sintaxa: statie <nume_statie>\n");
      }
      else
      {
        strcpy(current_location, req.argv[0]);
        printf("[client] Statie schimbata, se trimite comanda de tip rute la server pt. a cere datele noii statii...\n");
        send_routes_request(sd);
        print_response(sd);
      }
      break;
    case ESTIMARE:
    case INTARZIERE:
      if(req.argc<2){
        printf("Sintaxa: [intarziere|estimare] <numar_tren> <numar_minute>\n");
        break;
      }
    default:
      write(sd, &req, sizeof(struct request));
      print_response(sd);
    }

    if (req.type == QUIT)
    {
      break;
    }
  }

  close(sd);
}

void send_routes_request(int fd)
{
  struct request initial;
  bzero(&initial, sizeof(struct request));
  strcpy(initial.location, current_location);
  initial.type = RUTE;
  write(fd, &initial, sizeof(struct request));
}

void print_response(int fd)
{
  int lines;
  read(fd, &lines, sizeof(int));
  // printf("Received %d lines\n", lines);
  char line[RESPONSE_LINE_BUFFER_SIZE];
  // printf("RASPUNS:");
  for (int i = 0; i < lines; i++)
  {
    read(fd, &line, RESPONSE_LINE_BUFFER_SIZE);
    printf("%s\n", line);
  }
}

command_type get_command_type(char command[])
{
  if (strcmp(command, "rute") == 0)
  {
    return RUTE;
  }
  else if (strcmp(command, "sosiri") == 0)
  {
    return SOSIRI;
  }
  else if (strcmp(command, "plecari") == 0)
  {
    return PLECARI;
  }
  else if (strcmp(command, "intarziere") == 0)
  {
    return INTARZIERE;
  }
  else if (strcmp(command, "estimare") == 0)
  {
    return ESTIMARE;
  }
  else if (strcmp(command, "statie") == 0)
  {
    return STATIE;
  }
  else if (strcmp(command, "quit") == 0)
  {
    return QUIT;
  }
  else if (strcmp(command, "help") == 0)
  {
    return HELP;
  }
  else
  {
    return ERROR;
  }
}

struct request get_request(char commandline[])
{

  struct request req;
  bzero(&req, sizeof(struct request));
  strcpy(req.location, current_location);
  char command[20];
  sscanf(commandline, "%19s", command);
  req.type = get_command_type(command);
  if (req.type == STATIE)
  {

    regex statie("\\s*statie\\s+(\"([^\"]+)\"|([^\"\\s]+))\\s*");
    cmatch args;
    if (regex_match(commandline, args, statie))
    {
      const char *match1 = args.str(1).c_str();
      const char *match2 = args.str(2).c_str();

      if (match1[0] == '\"')
      {
        strncpy(req.argv[0], match2, COMMNANDLINE_BUFFER_SIZE);
      }
      else
      {
        strncpy(req.argv[0], match1, COMMNANDLINE_BUFFER_SIZE);
      }
      req.argc++;
      // printf("%s\n", req.argv[0]);
    }
  }
  else if (req.type == INTARZIERE || req.type==ESTIMARE)
  {
    regex intarziere("\\s*(intarziere|estimare)\\s+(\\d+)\\s+(\\d+)\\s*");
    cmatch args;
    if (regex_match(commandline, args, intarziere))
    {
      const char *match1 = args.str(2).c_str();
      const char *match2 = args.str(3).c_str();
      strncpy(req.argv[0], match1, COMMNANDLINE_BUFFER_SIZE);
      strncpy(req.argv[1], match2, COMMNANDLINE_BUFFER_SIZE);
      req.argc += 2;
    }
  }

  // printf("Type: %d\n", req.type);
  return req;
}
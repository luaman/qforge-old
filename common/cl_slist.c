#include <stdlib.h>
#include <cl_slist.h>
#include <stdio.h>
#include <quakeio.h>
#include <string.h>

//Better watch out for buffer overflows
server_entry_t	slist[MAX_SERVER_LIST];

void Server_List_Init(void) { // Do this or everything else will sig11
	int i;
	
	for(i=0;i < MAX_SERVER_LIST;i++) {
		slist[i].server = '\0';
		slist[i].description = '\0';
		slist[i].ping = 0;
	}
}


void Server_List_Shutdown(void) {  // I am the liberator of memory.
	int i;
	
	for(i=0;i < MAX_SERVER_LIST;i++) {
		if (slist[i].server)
			free(slist[i].server);
		if (slist[i].description)
			free(slist[i].description);
	}
}
			

int Server_List_Set(int i,char *addr,char *desc) {
	int len;
	if (i < MAX_SERVER_LIST && i >= 0) {
		if (slist[i].server)	// (Re)allocate memory first
			free(slist[i].server);
		if (slist[i].description)
			free(slist[i].description);
		len = strlen(desc);
		slist[i].server = malloc(strlen(addr) + 1);
		slist[i].description = malloc(len + 1);
		strcpy(slist[i].server,addr);
		strncpy(slist[i].description,desc,len);
		slist[i].description[len] = '\0'; //In case it got cut off
		return 0;  // Yay, we haven't segfaulted yet.
	}
	return 1; // Out of range
}

int Server_List_Load (QFile *f) { // This could get messy
	int serv = 0;
	char line[256]; // Long lines get truncated.
	char c = ' ';
	char *start;
	int len;
	int i;
	char *addr;

	// Init again to clear the list
	Server_List_Shutdown();
	Server_List_Init();
	while (serv < MAX_SERVER_LIST) {
		//First, get a line
		i = 0;
		c = ' ';
		while (c != '\n' && c != EOF) {
			c = Qgetc(f);
			if (i < 255) {
				line[i] = c;
				i++;
			}
		}
		line[i - 1] = '\0'; // Now we can parse it
		if ((start = gettokstart(line,1,' ')) != NULL) {
			len = gettoklen(line,1,' ');
			addr = malloc(len + 1);
			strncpy(addr,&line[0],len);
			addr[len] = '\0';
			if ((start = gettokstart(line,2,' '))) {
				Server_List_Set(serv,addr,start);
			}
			else {
				Server_List_Set(serv,addr,"Unknown");
			}
			serv++;
		} 
		if (c == EOF)  // We're done
			return 0;
	}
	return 0;  //I'd love to see the day when there are enough servers
}		   //for it to break out of the first while loop

char *gettokstart (char *str, int req, char delim) {
	char *start = str;
	
	int tok = 1;

	while (*start == delim) {
		start++;
	}
	if (*start == '\0')
		return '\0';
	while (tok < req) { //Stop when we get to the requested token
		if (*++start == delim) { //Increment pointer and test
			while (*start == delim) { //Get to next token
				start++;
			}
			tok++;
		}
		if (*start == '\0') {
			return '\0';
		}
	}
	return start;
}

int gettoklen (char *str, int req, char delim) {
	char *start = 0;
	
	int len = 0;
	
	start = gettokstart(str,req,delim);
	if (start == '\0') {
		return 0;
	}
	while (*start != delim && *start != '\0') {
		start++;
		len++;
	}
	return len;
}
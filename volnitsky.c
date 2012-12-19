/* A plain C implementation of Leonid Volnitsky's strstr() algorithm. 
   Implementation by David Turover, 2011.
   My contributions are public domain; Volnitsky's are whatever his terms are. 
   Volnitsky's algorithm is described at: 
   http://volnitsky.com/project/str_search/
*/ 


#include <string.h> 
char * volnitsky(char * s, int s_len, char * ss, int ss_len){

	typedef short word;
	typedef int offset; 

	#define HASH_MASK 0x1fff
	#define HASH_SIZE (HASH_MASK+1)
	#define HASH_MISS -1

	#define HASH_KEY(ptr) (*(word*)(ptr) & HASH_MASK)
	#define KEY_NEXT(k) ((k+1) & HASH_MASK)
	/* Use the 2^n-1 hash size as a binary mask */ 

	offset step = ss_len-1;
	char *ptr;
	short i;

	offset hash[HASH_SIZE];
	short key = 0;


        /* Test inputs. */
        if(s == NULL || s_len <= 0 || ss_len > s_len) return NULL;
        if(ss == NULL || ss_len <= 0) return s;

        /* For small strings, run a simpler algorthm. */
        if(ss_len == s_len) return strncmp(s, ss, ss_len)?NULL:s;
        if(ss_len < 3) return strstr(s, ss);

	/* Large substrings can exceed our data types. */
	if(ss_len > (HASH_MASK>>2) || (ss_len>>((long long)8*sizeof(offset)-1)) > 1){
		return strstr(s, ss); /* Fall back. */
	}


	/* Seed the hash. */
	memset(hash, HASH_MISS, sizeof(offset) * HASH_SIZE);
	for(i=0; i<=ss_len-sizeof(word); i++){
		key = HASH_KEY(ss+i);
		while(hash[key] != HASH_MISS){ key = KEY_NEXT(key); }
		hash[key] = i;
	}

	/* Search for a matching portion of the string. */
	for(ptr=s+step-1; ptr-s < s_len; ptr+=step){
		for(key=HASH_KEY(ptr); hash[key]!=HASH_MISS; key=KEY_NEXT(key)){
			if(strncmp(ptr-hash[key],ss,ss_len)==0){
				return ptr-hash[key];
			}
		}
	}

	return NULL; 

	#undef HASH_MASK
	#undef HASH_MISS
	#undef HASH_KEY
}




/* Test the function by searching for a word near the end of a large file. */
#include <stdlib.h>
#include <stdio.h>
int main(){
	FILE *f = fopen("/home/thunder/Documents/Link to Dropbox/UNI/Unix/A6_20080171071/big.txt","r");
	char *s = calloc(5120000, sizeof(char)); /* 4950996 on disk.*/
	char *test1 = "hello"; /* Near end of file to maximize differences */
	char *testa = "1080"; /* First word in file. */
	char *testz = "\nZZZ"; /* Last word in file */
	char *testlongestword = "hello";

	char *ss = test1; 
	const int read_size = 65536;
	int index = 0;
	int chars_written = 0;
	char *result; 
	if(f == NULL) return -1; 
	while(read_size == (chars_written = fread(s+index, sizeof(char), read_size, f))){
		index += chars_written;
	}

	/* Test a method of testing whether a 4-byte value overflows 2 bytes
           without knowing that they are 4-byte and 2-byte values */
	if((long)65536 > (unsigned short)(-1)) {
		printf("Test works. \n");
	} else {
		printf("They sign-extended it.\n");

	}

	result = volnitsky(s, index + chars_written, ss, strlen(ss));
	if(result){
		*(result + 60) = '\0';
		printf("Found something at:\n%s\n", result);
	} else {
		printf("Substring not found.\n");
	}

	return 0;
}


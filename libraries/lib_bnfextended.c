/**
 * @file lib_bnf_extended.c
 * @author Kishore
 * @date October 6, 2015, 12:10 AM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/headers.h"
#include "headers/lib_bnf.h"
#include "headers/lib_bnfextended.h"
#include "headers/lib_ncanonical.h"
#include "headers/lib_terminal.h"
#include "headers/lib_utilities.h"


enum bnfextended_nterminals{
	BNFEXTENDED_NUMBER = 11,
	BNFEXTENDED_STRING,
	BNFEXTENDED_VARIABLENAME
};

static int bnfextended_isnterm(char decision);
static bnfextended_t* __bnfextended_compile(char **bnfs, int n_bnfs, char *basebnf, int parsed);
static int bnfextended_nterm(char decision, char *string);
static int equal_so_far(char *s1, char *s2, int* isend);


/**
 * Adds bnf to the pointer list.
 * @param p : the pointer to which the bnf should be added to.
 * @param bnf : bnf string to be added.
 * @returns pointer to the list of bnfs.
 */
bnfextended_pointer_t* bnfextended_add(bnfextended_pointer_t *p, char *bnf)
{
	char *buffer = (char*)malloc(strlen(bnf)+1);
	int i=0, index=0;
	while (1)
	{
		if (bnf[i]=='<'){
			if (U_streq("<number>", bnf+i))
			{
				buffer[index++] = BNFEXTENDED_NUMBER;
				i += strlen("<number>");
				continue;
			}
			else if (U_streq("<string>", bnf+i))
			{
				buffer[index++] = BNFEXTENDED_STRING;
				i+=strlen("<string>");
				continue;
			}
			else if (U_streq("<variable", bnf+i))
			{
				buffer[index++] = BNFEXTENDED_VARIABLENAME;
				i+=strlen("<variable>");
				continue;
			}
		}
		else if (bnf[i]=='\0'){
			buffer[index++] = '\0';
			break;
		}
		buffer[index++] = bnf[i++];
	}
	bnfextended_pointer_t* new = (bnfextended_pointer_t *)malloc(sizeof(bnfextended_pointer_t)), *tmp = p;
	if (tmp!=NULL)
	{
		for(tmp=p; tmp->next!=NULL ; tmp=tmp->next);
		tmp->next = new;
		new->str = buffer;
		return p;
	}
	new->next = NULL;
	new->str = buffer;
	return new;
}


/**
 * Compiles the pointers list to bnfextended_t for the fast processing
 * when the string is parsed against it.
 * @param pointer : The pointer list that needs to be compiled
 * @returns bnfextended_t the compiled version of pointer list.
 */
bnfextended_t* bnfextended_compile(bnfextended_pointer_t *pointer)
{
	int len=0, i;
	bnfextended_pointer_t *ptr;
	for (ptr=pointer; ptr!=NULL; ptr=ptr->next)len++;
	
	char **bnfs = malloc(len*(sizeof(char*)));
	for (i=0, ptr=pointer; i<len; i++)
	{
		bnfs[i] = ptr->str;
		ptr = ptr->next;
	}
	__bnfextended_compile(bnfs, len, bnfs[0], 0);
}

/**
 * Returns parse state of input against start.
 * @param start : compiled version of pointer list used for parsing.
 * @param input : string that needs to be parsed against bnfextended.
 * @param terminal : if true writes the suggestion using terminal library,
 * and do not write if false.
 * @returns the status of input parsed against start.
 */
int bnfextended_suggest(bnfextended_t *start, char *input, int terminal)
{
	int parsed = 0;
	while (parsed<=strlen(input))
	{
		//printf("\r%.*s : %s : %s : %s\n", parsed, input, input, start->common, start->decision);
		if (start->common!=NULL){
			int isend = 0;
			int esf = equal_so_far(start->common, input+parsed, &isend);
			if (input[parsed+esf]=='\0')
			{
				char ch[2] = "a";
				int i=0, p=0;

				for (i=isend; (start->common[i]!='\0') && !(bnfextended_isnterm(start->common[i])); i++){
					p++;
					ch[0] = start->common[i];
					if (terminal)
						ncanonical_append(ch, 1);
				}
				if (p==0 && terminal)
				{
					printf("\a");
					fflush(stdout);
				}
				if (p!=0)
					return BNFEXTENDED_SUGGESTION_FOUND;
				else
					return BNFEXTENDED_SUGGESTION_NOT_FOUND;
			}
			else
			{
				if (start->common[isend]!='\0'){
					printf("\a");
					fflush(stdout);
					return BNFEXTENDED_SUGGESTION_NOT_FOUND;
				}
			}
			parsed += esf;
		}
		if (start->isend)
		{
			if (input[parsed]=='\0'){
				if (terminal){
					printf("\a");
					fflush(stdout);
				}
				return BNFEXTENDED_COMPLETE;
			}
		}
		if (start->decision!=NULL){
			char decision = input[parsed];
			if (decision=='\0'){
				if (terminal){
					printf("\a");
					fflush(stdout);
				}
				return BNFEXTENDED_SUGGESTION_NOT_FOUND;
			}
			int i=0, ok=0;
			for (i=0; start->decision[i]!='\0'; i++)
			{
				int p = bnfextended_nterm(start->decision[i], input+parsed);
				if (p!=0)
				{
					ok=1;
					start = start->next[i];
					parsed += p;
					break;
				}
			}
			
			if (!ok)
			{
				if (terminal){
					printf("\a");
					fflush(stdout);
				}
				return BNFEXTENDED_EXCEEDED;
			}
		}

	}
	assert(0);

}

/**
 * Prints the bnfextended_t tree on screen.
 * @param start : bnfextended_t.
 * @param nest : initial value should be 0.
 */
void bnfextended_print(bnfextended_t *start, int nest)
{
	
	if (start!=NULL)
	{
		if (start->common!=NULL)
		{
			int i=0;
			for (i=0; i<nest; i++)
			{
				printf("|-");
			}
			kprintf(start->common);
			//printf("\n");
			if (start->isend)
			{
				ansi_fg_bg_text(C_RED, C_DEFAULT, " END\n");
			}
			else
			{
				printf("\n");
			}
			nest += i;
		}
		if (start->decision!=NULL)
		{
			int j;
			for (j=0; start->decision[j]!='\0'; j++)
			{
				int i=0;
				for (i=0; i<nest; i++)
				{
					printf("|-");
				}
				char ch[3]="a\n";
				ch[0]=start->decision[j];
				kprintf(ch);
				bnfextended_print(start->next[j], nest+1);
			}
		}
	}
}


static int equal_so_far(char *s1, char *s2, int* isend)
{
	int i=0, index=0;
	while(1)
	{
		int p = bnfextended_nterm(s1[i], s2+index);
		
		if (p==0){
			*isend = i;
			return index;
		}
		else{
			index += p;
			i += 1;
		}
	}
	return i;
}

static int bnfextended_isnterm(char decision)
{
	switch(decision)
	{
		case BNFEXTENDED_NUMBER:
			return 1;
		case BNFEXTENDED_STRING:
			return 1;
		case BNFEXTENDED_VARIABLENAME:
			return 1;
		default:
			return 0;
	}
}

static int bnfextended_nterm(char decision, char *string)
{
	if (decision=='\0')
		return 0;
	else if (decision==BNFEXTENDED_NUMBER){
		int j=0;
		while (isdigit(string[j]) && (string[j]!='\0')){
			j++;
		}
		return j;
	}
	else if (decision==BNFEXTENDED_STRING){
		int j=0;
		while (isalpha(string[j]) && (string[j]!='\0')){
			j++;
		}
		return j;
	}
	else if (decision==BNFEXTENDED_VARIABLENAME){
		int j=0;
		if ((isalpha(string[j]) || string[j]=='_') && (string[j]!='\0')){
			j++;
		}
		else
			return 0;
		while ((isalnum(string[j]) || string[j]=='_') && (string[j]!='\0'))
		{
			j++;
		}
		return j;
	}
	if (decision==string[0])
		return 1;
	return 0;
}

static bnfextended_t* __bnfextended_compile(char **bnfs, int n_bnfs, char *basebnf, int parsed)
{
	bnfextended_t *start = (bnfextended_t *)malloc(sizeof(bnfextended_t));
	start->common = NULL;
	start->decision = NULL;
	start->isend = 0;
	start->next = NULL;

	int commonindexes[n_bnfs], commonindexes_len=0, i;
	
	for (i=0; i<n_bnfs; i++)
	{
		if (!strncmp(bnfs[i], basebnf, parsed)){
			commonindexes[commonindexes_len++] = i;
		}
	}

	assert(commonindexes_len!=0);

	if (commonindexes_len==1){
		assert(bnfs[commonindexes[0]]==basebnf);
		start->isend = 1;
		if (basebnf[parsed]=='\0')
		{
			start->common = NULL;
		}
		else
		{
			start->common = (char*)malloc(strlen(basebnf)-parsed+1);
			strcpy(start->common, basebnf+parsed);
		}
		return start;
	}
	int commonparsed = parsed;
	while (1)
	{
		int i;
		//sleep(1);
		char common = basebnf[commonparsed];
		int iscommon = 1;
		for (i=0; i<commonindexes_len; i++)
		{
			if (bnfs[commonindexes[i]][commonparsed]!=common)
			{
				iscommon=0;
				break;
			}
		}
		if (!iscommon)
			break;
		commonparsed++;
	}
	if (commonparsed!=parsed)
	{
		start->common = (char*)malloc(commonparsed-parsed+1);
		memcpy(start->common, basebnf+parsed, commonparsed-parsed);
		start->common[commonparsed-parsed] = '\0';
	}
	
	parsed = commonparsed;

	char diff[commonindexes_len+1], len=0;
	bnfextended_t *nexts[commonindexes_len];
	for (i=0; i<commonindexes_len; i++)
	{
		int k=0, found=0;
		if (bnfs[commonindexes[i]][parsed]=='\0'){
			start->isend=1;
			continue;
		}
		for (k=0; k<len; k++)
		{
			if (diff[k]==bnfs[commonindexes[i]][parsed])
			{
				found=1;
				break;
			}
		}
		if (!found)
		{
			diff[len] = bnfs[commonindexes[i]][parsed];
			nexts[len++] = __bnfextended_compile(bnfs, n_bnfs, bnfs[commonindexes[i]], parsed+1);
		}
		diff[len]='\0';
		int l=0;
		for (l=0; l<len; l++)
		{
			int k = 0;
			for (k=l; k<len; k++)
			{
				if (diff[k]>diff[l])
				{
					char tmp = diff[k];
					diff[k] = diff[l];
					diff[l] = tmp;
					
					bnfextended_t *t = nexts[k];
					nexts[k] = nexts[l];
					nexts[l] = t;
				}
			}
		}
		start->decision = (char*)malloc(len+1);
		strcpy(start->decision, diff);
	}
	start->next = (bnfextended_t **)malloc(sizeof(bnfextended_t **)*len);
	for (i=0; i<len; i++){
		start->next[i] = nexts[i];
	}
	return start;
}
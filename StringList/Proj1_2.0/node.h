#pragma once
#define NODE char*[3]/* NODE[0] is a pointer to previous node, 
						NODE[1] is a pointer to string which we wanna save(our data),
						NODE[2] is a pointer to next node*/

#define getData(ptr) (ptr)[1]// Macros for returning data from node

char  error_msg[] = "EROR: not found \n";
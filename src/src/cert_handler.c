#include "exim.h"
#include "cert_handler.h"

#define SIZE 26

Hash_element* hash_table[SIZE];

/*
   currently only a test method.
   Idea: textfile or db with certificates in pem format -> load from file 
*/
BOOL load_certificates(void) {
   uschar *testAddress1 = US"joachim@testmail";
   uschar *testAddress2 = US"joachim@mail1";
   uschar *testAddress3 = US"joachim@mail2";
   uschar *testcert1 = US"testcert1_joachim@testmail";
   uschar *testcert2 = US"testcert2_joachim@mail1";
   uschar *testcert3 = US"testcert3_joachim@mail2";
   insert(testAddress1, testcert1);
   insert(testAddress2, testcert2);
   insert(testAddress3, testcert3);
}

/*
    search for an certificate given the mail address in receiver_address and save it at the address given in cert
    
    Arguments:
         receiver_address  Mail address for which we want the certificate 
         cert              address where the certificate will be placed  

    Return:
        TRUE if an certificate for this address exists 
        FALSE if no certificate for this address exists
*/
BOOL search_and_get_recipient_cert(uschar *receiver_address, uschar ** cert) 
{
//return US"THIS IS A TEST CERT";
   DEBUG(D_transport) {
      debug_printf("<-------- searching for %s certificate -------> \n", receiver_address);
   }
   Hash_element *element = search_hash_table_index(receiver_address);
   if (!(element == NULL)) {
	Hash_element *debug_element = element;
	DEBUG(D_transport) {
      		debug_printf("		List at hash index:\n		");
	}
	do {
		DEBUG(D_transport) {
			debug_printf("%s -> ", debug_element->address);
		}
		debug_element = debug_element->next;
	} while(!(debug_element==NULL));
	debug_printf("\n");
   }
    
   uschar *cert_result = search_value_in_list(element, receiver_address);
   if(!(cert_result == NULL)) {
      *cert = cert_result;
      return TRUE;
   } else {
      DEBUG(D_transport) {
         debug_printf("    no certificate found for %s\n", receiver_address);
      }
      return FALSE;
   }
}

int hashNumber(int key) {
   return key % SIZE;
}

int hashString(uschar *key) {
   return tolower(key[0])-'a';
}

uschar * search_value_in_list(Hash_element * element, uschar *key) {
   while(!(element == NULL)) {
      DEBUG(D_transport) {
	 uschar *addr = element->address;
         debug_printf("    element address %s\n", addr);
      }
      if(strcmp(element->address, key)) {
         return element->cert; 
      }
      element=element->next;
   }        
	
   return NULL;        
}

Hash_element *search_hash_table_index(uschar *key) {
   //get the hash 
   int hashIndex = hashString(key);  
   DEBUG(D_transport) {
      debug_printf("    index in hash table: %d\n", hashIndex);
   }
   return hash_table[hashIndex];
}

void insert(uschar *key,uschar *data) {

   Hash_element *item = (Hash_element*) malloc(sizeof(Hash_element));
   item->cert = data;  
   item->address = key;
   item->next = NULL;

   //get the hash 
   int hashIndex = hashString(key);

   Hash_element *element = hash_table[hashIndex];
   DEBUG(D_transport) {
      debug_printf("    add element	 %s : %s\n", item->address, item->cert);
   }
  	 

   if(element == NULL) {
      hash_table[hashIndex] = item;
   } else {
      while(element->next != NULL) {
	DEBUG(D_transport) {
      		debug_printf("    	 %s -> \n", element->address);
   	}
         element = element->next;

      }
	
      element->next = item;
   }
}

// struct DataItem* delete(struct DataItem* item) {
//    int key = item->key;

//    //get the hash 
//    int hashIndex = hashCode(key);

//    //move in array until an empty
//    while(hashArray[hashIndex] != NULL) {
	
//       if(hashArray[hashIndex]->key == key) {
//          struct DataItem* temp = hashArray[hashIndex]; 
			
//          //assign a dummy item at deleted position
//          hashArray[hashIndex] = dummyItem; 
//          return temp;
//       }
		
//       //go to next cell
//       ++hashIndex;
		
//       //wrap around the table
//       hashIndex %= SIZE;
//    }      
	
//    return NULL;        
// }




typedef struct Hash_element{
   struct Hash_element *next;
   uschar *address;
   uschar *cert;
} Hash_element;


extern BOOL search_and_get_recipient_cert(uschar *receiver_address, uschar *cert);

extern BOOL load_certificates(void);

int hashNumber(int key);

int hashString(uschar *key);

uschar * search_value_in_list(Hash_element * element, uschar *key);

Hash_element *search_hash_table_index(uschar *key);

void insert(uschar *key,uschar *data);

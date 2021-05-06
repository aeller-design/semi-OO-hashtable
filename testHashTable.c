/*
 * A general purpose C unit testing framework developed for CPE 357 and
 * provided to students to expose them to and encourage their use of unit
 * testing and test driven development (TDD). While it was provided in
 * conjunction with the hash table project and associated exercise(s), students
 * are welcome and encourage to use and/or modify it, as they see fit, on
 * subsequent assignments in CPE 357 or anywhere else it they find it useful.
 *
 * Author: Kurt Mammen
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include "unitTest.h"
#include "hashTable.h"

#define TEST_ALL -1
#define REGULAR -2 
#define SPECIAL -3

#define MAX_RANDOM_STR 72 /* Max length of any random string */

/* Prototype for all test functions. This allows the creation of an array of
 * function pointers which makes the testing code shorter and more clear. It
 * also makes it easier/faster to add new tests - NICE!
 */
typedef void (*TestFunc)();

/* Structure for a test. See initRegularTests and initSpecialTests to see
 * how it is used.
 */
typedef struct
{
   TestFunc fn;
   char *fnName;
} Test;

/* Modified hash algorithm from K&R (page 144). This version does NOT mod the
 * calculated hash value by the table size because it does not know the table
 * size - the caller (the hash table itself) does though!
 *
 * Also note the following:
 *   * This hash only works for C strings.
 *   * Not the best/fastest/most flexible hash - you'll want something better
 *     for the Word Frequency project.
 */
static unsigned hashString(const void *data)
{
   unsigned hash;
   const char *str = data;

   for (hash = 0; *str;  str++)
      hash = *str + 31 * hash;

   return hash;
}
/*
static unsigned hashString(const void *data)
{
   const char *str = data;
   unsigned long hash = 5381;
   int c;

   while (c )
   {
      c = *str++;
      hash = ((hash << 5) + hash) + c; 
   }   

   return hash;
}                                                              
*/
static unsigned hashBad(const void *data)
{
   return 0;
}

static int compareString(const void *a, const void *b)
{
   return strcmp(a, b);
}

/* Helper function to make random strings for robust testing. Strings will be
 * a minimum of 3 characters up to MAX_RANDOM_STR characters in length.
 */
static char* randomString()
{
   char *str;
   int i;
   int length = (rand() % MAX_RANDOM_STR - 2) + 3;

   if (NULL == (str = malloc(length + 1)))
   {
      perror("randomString()");
      exit(EXIT_FAILURE);
   }

   /* Put random but visible char values in the string */
   for (i = 0; i < length; i++)
      str[i] = (rand() % (' ' - '~' + 1)) + ' ';

   str[length] = 0; /* nul-terminate it! */

   return str; 
}

/* PROVIDED TEST (do not modify)
 *
 * This is a provided sample test that matches the Evaluation System's English
 * test description for the Hash Table project's core test01. I hope it is
 * obvious to you that this is but one of many many variations that one could
 * write that would satisfy the English test description. Recall that one of
 * the important goals of the Evaluation System is to provide you with guidance
 * on how to test you solution but still allow you the opportunity develop your
 * own tests in the hopes that doing so will create an efficient and fruitful
 * learning experience!
 */
static void core01()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, -987, 0.73));
}

static void core02()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 0, 0.73));
}

static void core03()
{
   unsigned sizes[] = {0, 5};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 0.73));
}

static void core04()
{
   unsigned sizes[] = {1,2,2,5};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 0.73));
}

static void core05()
{
   unsigned sizes[] = {1,2,1,5};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 0.73));
}

static void core06()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 1, -0.73));
}
static void core07()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 1, 0));
}
static void core08()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 1, 1.01));
}
static void core09()
{
   unsigned sizes[] = {7, 11};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   /*void *data = malloc(sizeof(int));*/

   TEST_ERROR(htAdd(ht, NULL));
}
static void core10()
{
   unsigned sizes[] = {7, 11};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   TEST_ERROR(htLookUp(ht, NULL));
}
/* PROVIDED TEST (do not modify)
 *
 * This is a provided sample test that matches the Evaluation System's English
 * test description for the Hash Table project's core test11. I hope it is
 * obvious to you that this is but one of many many variations that one could
 * write that would satisfy the English test description. Recall that one of
 * the important goals of the Evaluation System is to provide you with guidance
 * on how to test you solution but still allow you the opportunity develop your
 * own tests in the hopes that doing so will create an efficient and fruitful
 * learning experience!
 */ 
static void core11()
{
   unsigned sizes[] = {31};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 1, 0.73);

   /* Do the test(s) */
   TEST_UNSIGNED(htCapacity(ht), 31);
   TEST_UNSIGNED(htUniqueEntries(ht), 0);
   TEST_UNSIGNED(htTotalEntries(ht), 0);

   htDestroy(ht);
}

/* PROVIDED TEST (do not modify)
 *
 * This is a provided sample test that matches the Evaluation System's English
 * test description for the Hash Table project's core test12. I hope it is
 * obvious to you that this is but one of many many variations that one could
 * write that would satisfy the English test description. Recall that one of
 * the important goals of the Evaluation System is to provide you with guidance
 * on how to test you solution but still allow you the opportunity develop your
 * own tests in the hopes that doing so will create an efficient and fruitful
 * learning experience!
 */ 
static void core12()
{
   int freq = 0;
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   /* Single entry */
   freq = htAdd(ht, randomString());

   TEST_UNSIGNED(freq, 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);
   
   htDestroy(ht);
}

static void core13()
{
   int freq = 0;
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   freq = htAdd(ht, randomString());
   freq = htAdd(ht, randomString());

   TEST_UNSIGNED(freq, 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);
                                     
   htDestroy(ht);
}





static void core14()
{
   int freq = 0;
   HTFunctions funcs = {hashString, compareString, NULL};
   unsigned sizes[] = {7};
   char *s2;
   void *ht;

   char *s1 = malloc(strlen("foo") + 1);
   if (NULL == s1)
   {
      perror("core14()");
      exit(EXIT_FAILURE);
   }
   s1 = strcpy(s1, "foo");

   s2 = malloc(strlen("foo") + 1);
   if (NULL == s2)
   {
      perror("core14()");
      exit(EXIT_FAILURE);
   }
   s2 =strcpy(s2, "foo");

   ht = htCreate(&funcs, sizes, 1, .73);

   freq = htAdd(ht, s1);

   TEST_UNSIGNED(freq, 1);

   freq = htAdd(ht, s2);

   TEST_UNSIGNED(freq, 2);
   TEST_UNSIGNED(htCapacity(ht), 7);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   free(s2);
   
   htDestroy(ht);
}

static void core15()
{
   int freq = 0;
   HTFunctions funcs = {hashBad, compareString, NULL};
   unsigned sizes[] = {7};
   char *s2;
   void *ht;

   char *s1 = malloc(strlen("foo") + 1);
   if (NULL == s1)
   {
      perror("core15()");
      exit(EXIT_FAILURE);
   }
   s1 = strcpy(s1, "foo");

   s2 = malloc(strlen("bar") + 1);
   if (NULL == s2)
   {
      perror("core15()");
      exit(EXIT_FAILURE);
   }
   s2 = strcpy(s2, "bar");

   ht = htCreate(&funcs, sizes, 1, .73);

   htAdd(ht, s1);
   freq = htAdd(ht, s2);

   TEST_UNSIGNED(freq, 1);
   TEST_UNSIGNED(htCapacity(ht), 7);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);
   
   htDestroy(ht);
}

static void core16()
{
   HTEntry notThere;
   HTFunctions funcs = {hashBad, compareString, NULL};
   unsigned sizes[] = {7};
   void *ht = htCreate(&funcs, sizes, 1, .73);

   notThere = htLookUp(ht, "foo");
   
   TEST_BOOLEAN(notThere.data == NULL, 1);
   TEST_UNSIGNED(notThere.frequency, 0);

   htDestroy(ht);
}

static void core17()
{
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   char *str1 = malloc(strlen("a") + 1);
   char *str2 = malloc(strlen("b") + 1);
   char *str3 = malloc(strlen("c") + 1);
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   HTEntry entry;

   strcpy(str1, "a");
   strcpy(str2, "b");
   strcpy(str3, "c");

   htAdd(ht, str1);
   htAdd(ht, str2);
   htAdd(ht, str3);

   entry = htLookUp(ht, "d");
                                                 
   TEST_BOOLEAN(entry.data == NULL, 1);
   TEST_UNSIGNED(entry.frequency, 0);

   htDestroy(ht);
}

static void core18()
{
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   HTEntry entry;
   char *str1 = malloc(2);
   char *str2 = malloc(2);
   char *str3 = malloc(2);

   strcpy(str1, "a");
   strcpy(str2, "b");
   strcpy(str3, "c");

   htAdd(ht, str1);
   htAdd(ht, str2);
   htAdd(ht, str3);

   entry = htLookUp(ht, "a");
   TEST_STRING(entry.data, str1);
   TEST_BOOLEAN(entry.data == str1, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "b");
   TEST_STRING(entry.data, str2);
   TEST_BOOLEAN(entry.data == str2, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "c");
   TEST_STRING(entry.data, str3);
   TEST_BOOLEAN(entry.data == str3, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   htDestroy(ht);
}

static void core19()
{
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   HTEntry entry;
                   
   char *str1 = malloc(2);
   char *str2 = malloc(2);
   char *str3 = malloc(2);

   strcpy(str1, "a");
   strcpy(str2, "a");
   strcpy(str3, "a");
   
   htAdd(ht, str1);
   htAdd(ht, str2);
   htAdd(ht, str3);

   entry = htLookUp(ht, str2);
                                                 
   TEST_STRING(entry.data, "a");
   TEST_UNSIGNED(entry.frequency, 3);
   TEST_BOOLEAN(str1==entry.data, 1);
   TEST_BOOLEAN(str2==entry.data, 0);
   TEST_BOOLEAN(str3==entry.data, 0);

   free(str2);
   free(str3);
   htDestroy(ht);
}
/*FEATURE TESTS*/
static void feat01()
{
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashBad, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   HTEntry entry;

   char *str1 = malloc(2);
   char *str2 = malloc(2);

   strcpy(str1, "a");
   strcpy(str2, "b");

   htAdd(ht, str1);
   htAdd(ht, str2);
   htAdd(ht, str1);
   
   entry = htLookUp(ht, str1);
   TEST_STRING(entry.data, "a");
   TEST_UNSIGNED(entry.frequency, 2);

   entry = htLookUp(ht, str2);
   TEST_STRING(entry.data, "b");
   TEST_UNSIGNED(entry.frequency, 1);

   htDestroy(ht);
}

static void feat02()
{
   unsigned sizes[] = {3, 11, 17};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 3, 0.5);

   char *str1 = malloc(2);
   char *str2 = malloc(2);
   char *str3 = malloc(2);
   char *str4 = malloc(2);
   char *str5 = malloc(2);
   char *str6 = malloc(2);
   HTEntry entry;

   strcpy(str1, "a");
   strcpy(str2, "a");
   strcpy(str3, "b");
   strcpy(str4, "c");
   strcpy(str5, "d");
   strcpy(str6, "a");
fprintf(stderr,"adding %s\n", (char*)str1);
   htAdd(ht, str1);
fprintf(stderr,"adding %s\n", (char*)str1);
   htAdd(ht, str1);
fprintf(stderr,"adding %s\n", (char*)str2);
   htAdd(ht, str2);

   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 3);

   htAdd(ht, str3);

   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 4);
   
   htAdd(ht, str4);
   htAdd(ht, str5);

   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 6);
  
   entry = htLookUp(ht, "a");
   TEST_STRING(entry.data, "a");
   TEST_BOOLEAN(entry.data == str1, 1);
   TEST_UNSIGNED(entry.frequency, 3);
   
   entry = htLookUp(ht, "b");
   TEST_STRING(entry.data, "b");
   TEST_BOOLEAN(entry.data == str3, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "c");
   TEST_STRING(entry.data, "c");
   TEST_BOOLEAN(entry.data == str4, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "d");
   TEST_STRING(entry.data, "d");
   TEST_BOOLEAN(entry.data == str5, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   free(str2);
   htDestroy(ht);
}

static void feat03()
{
   unsigned sizes[] = {3, 11, 17};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 3, 0.5);

   char *str1 = malloc(strlen("a") + 1);
   char *str2 = malloc(strlen("a") + 1);
   char *str3 = malloc(strlen("a") + 1);
   char *str4 = malloc(strlen("a") + 1);
   char *str5 = malloc(strlen("a") + 1);
   char *str6 = malloc(strlen("a") + 1);
   char *str7 = malloc(strlen("a") + 1);
   char *str8 = malloc(strlen("a") + 1);
   HTEntry entry;

   strcpy(str1, "a");
   strcpy(str2, "b");
   strcpy(str3, "c");
   strcpy(str4, "d");
   strcpy(str5, "a");
   strcpy(str6, "e");
   strcpy(str7, "f");
   strcpy(str8, "g");

   htAdd(ht, str1);
   htAdd(ht, str1);
   htAdd(ht, str2);

   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 3);

   htAdd(ht, str3);

   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 3);
   TEST_UNSIGNED(htTotalEntries(ht), 4);
   
   htAdd(ht, str4);
   htAdd(ht, str5);

   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 6);

   htAdd(ht, str6);
   htAdd(ht, str7);

   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 6);
   TEST_UNSIGNED(htTotalEntries(ht), 8);

   htAdd(ht, str8);

   TEST_UNSIGNED(htCapacity(ht), 17);
   TEST_UNSIGNED(htUniqueEntries(ht), 7);
   TEST_UNSIGNED(htTotalEntries(ht), 9);

   entry = htLookUp(ht, "a");
   TEST_STRING(entry.data, "a");
   TEST_BOOLEAN(entry.data == str1, 1);
   TEST_UNSIGNED(entry.frequency, 3);

   entry = htLookUp(ht, "b");
   TEST_STRING(entry.data, "b");
   TEST_BOOLEAN(entry.data == str2, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "c");
   TEST_STRING(entry.data, "c");
   TEST_BOOLEAN(entry.data == str3, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "d");
   TEST_STRING(entry.data, "d");
   TEST_BOOLEAN(entry.data == str4, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "e");
   TEST_STRING(entry.data, "e");
   TEST_BOOLEAN(entry.data == str6, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "f");
   TEST_STRING(entry.data, "f");
   TEST_BOOLEAN(entry.data == str7, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "g");
   TEST_STRING(entry.data, "g");
   TEST_BOOLEAN(entry.data == str8, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   free(str5);
   htDestroy(ht);
}

static void feat04()
{
   unsigned sizes[] = {3};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 1, 0.5);

   char *str1 = malloc(strlen("a") + 1);
   char *str2 = malloc(strlen("a") + 1);
   char *str3 = malloc(strlen("a") + 1);
   char *str4 = malloc(strlen("a") + 1);
   char *str5 = malloc(strlen("a") + 1);
   HTEntry entry;

   strcpy(str1, "a");
   strcpy(str2, "b");
   strcpy(str3, "c");
   strcpy(str4, "d");
   strcpy(str5, "e");

   htAdd(ht, str1);
   htAdd(ht, str1);
   htAdd(ht, str2);

   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 3);

   htAdd(ht, str3);

   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 3);
   TEST_UNSIGNED(htTotalEntries(ht), 4);
   
   htAdd(ht, str4);
   htAdd(ht, str5);

   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 5);
   TEST_UNSIGNED(htTotalEntries(ht), 6);

   entry = htLookUp(ht, "a");
   TEST_STRING(entry.data, "a");
   TEST_BOOLEAN(entry.data == str1, 1);
   TEST_UNSIGNED(entry.frequency, 2);

   entry = htLookUp(ht, "b");
   TEST_STRING(entry.data, "b");
   TEST_BOOLEAN(entry.data == str2, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "c");
   TEST_STRING(entry.data, "c");
   TEST_BOOLEAN(entry.data == str3, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "d");
   TEST_STRING(entry.data, "d");
   TEST_BOOLEAN(entry.data == str4, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "e");
   TEST_STRING(entry.data, "e");
   TEST_BOOLEAN(entry.data == str5, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   htDestroy(ht);
}

static void feat05()
{
   unsigned sizes[] = {3, 11, 17};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 3, 1.0);

   char *str1 = malloc(2);
   char *str2 = malloc(2);
   char *str3 = malloc(2);
   char *str4 = malloc(2);
   char *str5 = malloc(2);
   HTEntry entry;

   strcpy(str1, "a");
   strcpy(str2, "b");
   strcpy(str3, "c");
   strcpy(str4, "d");
   strcpy(str5, "a");

   htAdd(ht, str1);
   htAdd(ht, str1);
   htAdd(ht, str2);

   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 3);

   htAdd(ht, str3);

   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 3);
   TEST_UNSIGNED(htTotalEntries(ht), 4);
   
   htAdd(ht, str4);
   htAdd(ht, str5);

   TEST_UNSIGNED(htCapacity(ht), 3);
   TEST_UNSIGNED(htUniqueEntries(ht), 4);
   TEST_UNSIGNED(htTotalEntries(ht), 6);

   entry = htLookUp(ht, "a");
   TEST_STRING(entry.data, "a");
   TEST_BOOLEAN(entry.data == str1, 1);
   TEST_UNSIGNED(entry.frequency, 3);

   entry = htLookUp(ht, "b");
   TEST_STRING(entry.data, "b");
   TEST_BOOLEAN(entry.data == str2, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "c");
   TEST_STRING(entry.data, "c");
   TEST_BOOLEAN(entry.data == str3, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   entry = htLookUp(ht, "d");
   TEST_STRING(entry.data, "d");
   TEST_BOOLEAN(entry.data == str4, 1);
   TEST_UNSIGNED(entry.frequency, 1);

   free(str5);
   htDestroy(ht);
}
static void feat06()
{
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   unsigned size = 0;
   HTEntry* entries;
   entries = htToArray(ht, &size);
   TEST_BOOLEAN(entries == NULL, 1);
   TEST_UNSIGNED(size, 0);
   
   
   htDestroy(ht);
   free(entries);
}
static void feat07()
{
   unsigned sizes[] = {3};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 1, 0.73);
   unsigned size = 0;
   HTEntry* entries;
   HTEntry  entry;

   char *str1 = malloc(2);
   char *str2 = malloc(2);
   char *str3 = malloc(2);
   char *str4 = malloc(2);
   char *str5 = malloc(2);

   strcpy(str1, "a");
   strcpy(str2, "b");
   strcpy(str3, "c");
   strcpy(str4, "d");
   strcpy(str5, "a");

   htAdd(ht, str1);
   htAdd(ht, str1);
   htAdd(ht, str2);
   htAdd(ht, str3);
   htAdd(ht, str4);
   htAdd(ht, str5);

   entries = htToArray(ht, &size);
   TEST_UNSIGNED(size, 4);

   entry = htLookUp(ht, "d");
   TEST_STRING(entry.data, "d");


   TEST_STRING(entries[0].data, "c");
   TEST_UNSIGNED(entries[0].frequency, 1);

   TEST_STRING(entries[1].data, "d");
   TEST_UNSIGNED(entries[1].frequency, 1);

   TEST_STRING(entries[2].data, "a");
   TEST_UNSIGNED(entries[2].frequency, 3);

   TEST_STRING(entries[3].data, "b");
   TEST_UNSIGNED(entries[3].frequency, 1);
   
   free(str5);
   htDestroy(ht);
   free(entries);
}

static void feat08()
{
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   HTMetrics metric;

   metric = htMetrics(ht);
  
   TEST_UNSIGNED(metric.numberOfChains, 0);
   TEST_UNSIGNED(metric.maxChainLength, 0);
   TEST_REAL(metric.avgChainLength, 0, FLT_EPSILON);
   
   htDestroy(ht);
}
static void feat09()
{
   unsigned sizes[] = {3};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 1, 0.73);
   HTMetrics metric;
   
   char *str1 = malloc(2);
   char *str2 = malloc(2);
   char *str3 = malloc(2);
   char *str4 = malloc(2);
   char *str5 = malloc(2);

   strcpy(str1, "a");
   strcpy(str2, "b");
   strcpy(str3, "c");
   strcpy(str4, "d");
   strcpy(str5, "a");

   htAdd(ht, str1);
   htAdd(ht, str1);
   htAdd(ht, str2);
   htAdd(ht, str3);
   htAdd(ht, str4);
   htAdd(ht, str5);
   
   metric = htMetrics(ht);


   
   TEST_UNSIGNED(metric.numberOfChains, 3);
   TEST_UNSIGNED(metric.maxChainLength, 2);
   TEST_REAL(metric.avgChainLength, ((float)4/3),  FLT_EPSILON);
   
   free(str5);
   htDestroy(ht);
}
static void feat10()
{
   unsigned sizes[] = {3};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 1, 0.73);
   HTMetrics metric;
   
   char *str1 = malloc(2);
   char *str2 = malloc(2);
   char *str3 = malloc(2);
   char *str4 = malloc(2);
   char *str5 = malloc(2);
   char *str6 = malloc(2);
   char *str7 = malloc(2);

   strcpy(str1, "a");
   strcpy(str2, "b");
   strcpy(str3, "c");
   strcpy(str4, "f");
   strcpy(str5, "g");
   strcpy(str6, "h");
   strcpy(str7, "i");

   htAdd(ht, str1);
   htAdd(ht, str1);
   htAdd(ht, str2);
   htAdd(ht, str3);
   htAdd(ht, str4);
   htAdd(ht, str5);
   htAdd(ht, str6);
   htAdd(ht, str7);
   
   metric = htMetrics(ht);

   TEST_UNSIGNED(metric.numberOfChains, 3);
   TEST_UNSIGNED(metric.maxChainLength, 2);
   TEST_REAL(metric.avgChainLength, ((float)4/3), FLT_EPSILON);
   
   htDestroy(ht);
}

static unsigned hash11(const void *data)
{
   return strlen((char*) data) % 3;
}


static void feat11()
{
       int i, j, k;
          int *sizeList = calloc(26, 4);
             char **strPoint = calloc(26, 8);
                HTEntry entry;

                   unsigned sizes[] = {11};
                      HTFunctions funcs = {hash11, compareString, NULL};
                         void *ht = htCreate(&funcs, sizes, 1, 0.73);
                            
                            for (i = 0; i < 26; i++) 
                                   {
                                             for (j = 0, k = rand() % 5 + 1; j <k; j++)
                                                       {
                                                                    char *str1 = malloc(2);
                                                                             char curStr[] = "a";
                                                                                      sizeList[i]++;
                                                                                               curStr[0] = curStr[0] + i;
                                                                                                        strcpy(str1, curStr);
                                                                                                                 htAdd(ht, str1);
                                                                                                                          if(sizeList[i] > 1)
                                                                                                                                       {
                                                                                                                                                       free(str1);
                                                                                                                                                                }
                                                                                                                                   else
                                                                                                                                                {
                                                                                                                                                                strPoint[i] = str1;
                                                                                                                                                                         }
                                                                                                                                         }
                                                }

                               for (i = 0; i<26; i++)
                                      {
                                                entry = htLookUp(ht, strPoint[i]);
                                                      TEST_STRING(entry.data, strPoint[i]);
                                                            TEST_BOOLEAN(entry.data == strPoint[i], 1);
                                                                  TEST_UNSIGNED(entry.frequency, sizeList[i]);
                                                                     }

                                  free(sizeList);
                                     free(strPoint);

                                        htDestroy(ht);
}




typedef struct
{
       unsigned *x;
} unsignedPtr;

static unsigned hashInt(const void *data)
{
       return (*((unsignedPtr *)data)->x);
}

static int compareInt(const void *a, const void *b)
{
       const unsignedPtr *ptr1 = a;
          const unsignedPtr *ptr2 = b;
             return *(ptr1->x) - *(ptr2->x);
}

static void unsignedDestroy(const void *data)
{
       const unsignedPtr *ptr = data;
          free(ptr->x);
}

static void feat12()
{
       unsigned i, j, k, numEnt;

          unsigned sizes[] = {7};
             HTFunctions funcs = {hashInt, compareInt, unsignedDestroy};
                void *ht = htCreate(&funcs, sizes, 1, 0.73);
                   HTMetrics metrics;
                      HTEntry entry;
                         int *sizeList = calloc(26, 4);
                            unsignedPtr ptr1;
                             
                               numEnt = 0;
                                  for (i = 0; i<22; i++)
                                         {
                                                   for (j = 0, k = rand() % 5+1; j <k; j++)
                                                             {
                                                                          unsignedPtr *ptr = malloc(sizeof(unsignedPtr));
                                                                                   numEnt++;
                                                                                            sizeList[i] += 1;
                                                                                                     ptr->x = malloc(sizeof(unsigned));
                                                                                                              *(ptr->x) = i;
                                                                                                                       htAdd(ht, ptr);
                                                                                                                                if(sizeList[i] > 1)
                                                                                                                                             {
                                                                                                                                                             free(ptr->x);
                                                                                                                                                                         free(ptr);
                                                                                                                                                                                  }
                                                                                                                                      }
                                                      }

                                     i = 21;
                                        ptr1.x = &i;

                                           metrics = htMetrics(ht);
                                              TEST_UNSIGNED(metrics.maxChainLength, 4);
                                                 TEST_REAL(metrics.avgChainLength, 22.0/7, FLT_EPSILON);
                                                    TEST_UNSIGNED(metrics.numberOfChains, 7);

                                                       entry = htLookUp(ht, &ptr1);
                                                          TEST_BOOLEAN((compareInt)(entry.data, &ptr1), 0);
                                                             TEST_UNSIGNED(entry.frequency, sizeList[21]);

                                                                i = 11;
                                                                   entry = htLookUp(ht, &ptr1);
                                                                      TEST_BOOLEAN((compareInt)(entry.data, &ptr1), 0);
                                                                         TEST_UNSIGNED(entry.frequency, sizeList[11]);

                                                                            TEST_UNSIGNED(htTotalEntries(ht), numEnt);
                                                                               TEST_UNSIGNED(htUniqueEntries(ht), 22);
                                                                                  
                                                                                  free(sizeList);
                                                                                     htDestroy(ht);
}



static void feat13()
{
       unsigned i, j, k, numEnt;

          unsigned sizes[] = {3, 7};
             HTFunctions funcs = {hashInt, compareInt, unsignedDestroy};
                void *ht = htCreate(&funcs, sizes, 2, 0.73);
                   HTMetrics metrics;
                      HTEntry entry;
                         int *sizeList = calloc(26, 4);
                            unsignedPtr ptr1;
                             
                               numEnt = 0;
                                  for (i = 0; i<22; i++)
                                         {
                                                                 for (j = 0, k = rand() % 5+1; j <k; j++)
                                                                       {
                                                                                unsignedPtr *ptr = malloc(sizeof(unsignedPtr));
                                                                                         numEnt++;
                                                                                                  sizeList[i] += 1;
                                                                                                           ptr->x = malloc(sizeof(unsigned));
                                                                                                                    *(ptr->x) = i;
                                                                                                                             htAdd(ht, ptr);
                                                                                                                                      /*fprintf(stderr, " added int: %u\n", *(ptr->x));*/
                                                                                                                                      if(sizeList[i] > 1)
                                                                                                                                               {
                                                                                                                                                           free(ptr->x);
                                                                                                                                                                       free(ptr);
                                                                                                                                                                                }
                                                                                                                                                                                }
                                                                                                                                                                                         }

                                                                                                                                                                                            i = 21;
                                                                                                                                                                                               ptr1.x = &i;

                                                                                                                                                                                                  metrics = htMetrics(ht);
                                                                                                                                                                                                     TEST_UNSIGNED(metrics.maxChainLength, 4);
                                                                                                                                                                                                        TEST_REAL(metrics.avgChainLength, 22.0/7, FLT_EPSILON);
                                                                                                                                                                                                           TEST_UNSIGNED(metrics.numberOfChains, 7);

                                                                                                                                                                                                              entry = htLookUp(ht, &ptr1);
                                                                                                                                                                                                                 TEST_BOOLEAN((compareInt)(entry.data, &ptr1), 0);
                                                                                                                                                                                                                    TEST_UNSIGNED(entry.frequency, sizeList[21]);

                                                                                                                                                                                                                       i = 11;
                                                                                                                                                                                                                          entry = htLookUp(ht, &ptr1);
                                                                                                                                                                                                                             TEST_BOOLEAN((compareInt)(entry.data, &ptr1), 0);
                                                                                                                                                                                                                                TEST_UNSIGNED(entry.frequency, sizeList[11]);

                                                                                                                                                                                                                                   TEST_UNSIGNED(htTotalEntries(ht), numEnt);
                                                                                                                                                                                                                                      TEST_UNSIGNED(htUniqueEntries(ht), 22);
                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                         free(sizeList);
                                                                                                                                                                                                                                            htDestroy(ht);
}
static void performance()
{
   int i;
   unsigned sizes[] = {11, 131, 1001, 10001};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 4, 0.73);
   char* str;

   for(i = 0; i < 100000; i++)
   {
      str = randomString();
      if((htAdd(ht, str)) > 1)
         free(str);
   }

   htDestroy(ht);
}
static void testAll(Test* tests)
{
   int i;

   for (i = 0; tests[i].fn != NULL; i++)
   {
      printf("Running regular %s\n", tests[i].fnName);

      /* Call the test function via function pointer in the array */
      tests[i].fn();
   }
}

static void findAndCall(Test* tests, const char *type, char *fnName)
{
   int i;

   for (i = 0; tests[i].fn != NULL; i++)
   {
      if (0 == strcmp(tests[i].fnName, fnName))
      {
         /* Found it, call the function via function pointer... */
         printf("Running %s %s\n", type, fnName);
         tests[i].fn();
         return;
      }
   }

   fprintf(stderr, "ERROR %s %s: Function not found\n", type, fnName);
   exit(EXIT_FAILURE);
}

static void runTests(Test *tests, const char *type, char *fnName)
{
   if (fnName == NULL)
      testAll(tests);
   else
      findAndCall(tests, type, fnName);

   /* Free the tests (allocated in initTests) */
   free(tests);
}

static char* checkArgs(int argc, char *argv[], int *testType)
{
   char *testName;

   if (argc == 1)
   {
      *testType = REGULAR;      
      testName = NULL;
   }
   else if (argc == 2)
   {
      *testType = REGULAR; 
      testName = argv[1];
   }
   else if (argc == 3)
   {
      if (0 != strcmp(argv[1], "-special"))
      {
         fprintf(stderr, "Invalid option '%s'\n", argv[1]);
         exit(EXIT_FAILURE);
      }
      
      *testType = SPECIAL;
      testName = argv[2];
   }
   else
   {
      fprintf(stderr, "Usage: %s [testName | -special testName]\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   return testName;
}

Test* initTests(Test tests[], int size)
{
   Test *dynamicMemory = malloc(size);

   if (dynamicMemory == NULL)
   {
      fprintf(stderr, "FAILURE in %s at %d: ", __FILE__, __LINE__);
      perror(NULL);
      exit(EXIT_FAILURE);
   }

   return memcpy(dynamicMemory, tests, size);
}

/* Allocates, initializes, and returns the array of regular test functions.
 * Regular test functions are those that are expected to pass or report failure
 * BUT NOT terminate the test driver.
 *
 * By default, the test driver runs ALL of the regular tests. Alternatively, you
 * can run one at a time by simply specifying its name when you invoke the
 * test driver.
 *
 * See initSpecialTests for tests that you always want to run individually.
 *
 * NOTE: The last structure in the array must have NULL values as this indicates
 *    the array's end.
 *
 * IMPORTANT SUBTLETY: You can only use sizeof to obtain an array's size in
 *    the scope where the array is declared, otherwise you will just get
 *    the size of the pointer to the array.
 */
Test* initRegularTests()
{
   Test local[] = {
      {core11, "core11"},
      {core12, "core12"},
      {core13, "core13"},
      {core14, "core14"},
      {core15, "core15"},
      {core16, "core16"},
      {core17, "core17"},
      {core18, "core18"},
      {core19, "core19"},

      {feat01, "feature01"},
      {feat02, "feature02"},
      {feat03, "feature03"},
      {feat04, "feature04"},
      {feat05, "feature05"},
      {feat06, "feature06"},
      {feat07, "feature07"},
      {feat08, "feature08"},
      {feat09, "feature09"},
      {feat10, "feature10"},
      {feat11, "feature11"},
      {feat12, "feature12"},
      {feat13, "feature13"},
      {performance, "performance"},
      {NULL, NULL}
   };

   /* See IMPORTANT SUBTLETY above regarding the use of sizeof on arrays */
   return initTests(local, sizeof(local));
}

/* Allocates, initializes, and returns the array of special test functions.
 * Special test functions are those that you want to run individually for one
 * reason or another. For example, a test to see if a function asserts failure
 * when it is supposed to. Or, a test intended to measure CPU or memory
 * performance using tools like the Unix time command or Valgrind - for
 * tests like these you only want to run the single test and then observe the
 * results.
 *
 * See initRegularTests for tests that can together.
 *
 * NOTE: The last structure in the array must have NULL values as this indicates
 *    the array's end.
 *
 * IMPORTANT SUBTLETY: You can only use sizeof to obtain an array's size in
 *    the scope where the array is declared, otherwise you will just get
 *    the size of the pointer to the array.
 */
Test* initSpecialTests()
{
   Test local[] = {
      {core01, "core01"},
      {core02, "core02"}, 
      {core03, "core03"},
      {core04, "core04"},
      {core05, "core05"},
      {core06, "core06"},
      {core07, "core07"},
      {core08, "core08"},
      {core09, "core09"},
      {core10, "core10"},
      {NULL, NULL}
   };

   /* See IMPORTANT SUBTLETY above regarding the use of sizeof on arrays */
   return initTests(local, sizeof(local)); 
}

/* Test driver for Hash Table Project.
 *
 * Usage: testHashTable [functionName | -special functionName]
 *
 * When no option is specified ALL of the regular tests are run.
 * When "functionName" is specified that single regular test is run.
 * When "-special functionName" is specified that single special test is run.
 *
 * What TODO: As provided with the Unit Test Hash Table exercise, this unit test
 * driver provides several working examples of actual Evaluation System tests
 * for the Hash Table project. The exercise asks you to develop a few more so
 * that you can experience how easy it is to develop unit tests BEFORE you even
 * implement any actual code for the solution being tested. The hope is that
 * this will encourage you to develop tests early and often as you develop your
 * Hash Table project solution thereby adding unit testing and Test Driven
 * Development (TDD) to your personal repertoire of programming skills! 
 *    
 * To add a new test you must:
 * 
 *    1) Write a test function.
 *    2) Add its name to the appropriate array of test functions, see
 *       initRegularTests and initSpecialTests for details.
 *    3) Compile and test your test by verifying:
 *          1. It report failure when run on code known to be incorrect.
 *          2. It passes (does NOT report failure) when run on code believed to
 *             be correct.
 *    4) Be sure use Valgrind, where appropriate, to check for memory errors
 *       and/or leaks too!
 */
int main(int argc, char *argv[])
{
   char *testName;
   int testType;

   /* Random numbers used to produce "interesting" strings for testing */
   srand(182955);

   /* Make stdout unbuffered so that test output is synchronous on signals */
   setbuf(stdout, NULL);

   /* Get the test name type */
   testName = checkArgs(argc, argv, &testType);
 
   /* Run the test(s)... */
   if (testType == REGULAR)
      runTests(initRegularTests(), "regular", testName);
   else
      runTests(initSpecialTests(), "special", testName);
   
   return 0;
}

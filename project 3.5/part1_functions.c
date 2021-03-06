#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Calculates a value (c) based on the input parameters (a, b) and prints
 * out the result.
 *
 * @param a
 *     Input parameter a.
 *
 * @param b
 *     Input parameter b.
 */
void one(const int a, const int b) {
  int c = (a * a) + (b * b);
  printf("%d^2 + %d^2 = %d\n", a, b, c);
}

void two(const char *grade) {
  // you may find the atoi function useful
  
  int atoi_grade = atoi(grade);
  
  if (atoi_grade > 70){
    double atof_grade = atof(grade);
    printf("%f passed!\n", atof_grade);
  }else{
    printf("%d not passed!\n", atoi_grade);
  }
}

/**
 * Assigns a pointer (int *p) the value of a stack variable (int x).
 */
void three() {
  int x = 4;
  int *p = (int*) calloc (1, sizeof(int));
  *p=x;

  printf("The value of p is: %d\n", *p);
  free(p);
}

/**
 * Prints out a specific message based on if the number is
 * between 0 and 1 (exclusive).
 *
 * @param value
 *     Value to test.
 */
void four(const float value) {
  if(value>0){
    if (value<1){
      printf("The value is between zero and one.\n");
    } else{
      printf("The value is not between zero and one.\n");
    }
  }
}

/**
 * Prints out a specific message based on if the two input parameters
 * are equal.
 *
 * @param x
 *     First input parameter.
 *
 * @param y
 *     Second input parameter.
 */
void five(const int *x, const int *y) {
  if (*x == *y)
    printf("x and y are equal.\n");
  else
    printf("x and y are different.\n");
}

/**
 * Returns a new pointer (float *p) which contains the value of the
 * input pointer (int *x).
 *
 * @param x
 *     Input parameter, whose value will be returned as a (float *).
 *
 * @returns
 *     A new pointer, allocated on the heap and not freed, that
 *     contains the value of the input parameter.
 */
float* six(const int *x) {
  float *p = (float*) calloc(1,sizeof(float));
  *p = *x;            
  return p;
}

/**
 * Takes an input a and checks whether a is an alphabet, it can be
 * both uppercase and lowercase
 *
 * @param a
 *     Input parameter a, which is a char*
 *
 */
void seven(const char *a) {
  int asciicode = *a;
  if ((65<=asciicode && asciicode<=90) || (97<=asciicode && asciicode<=122)){
    printf("a is a letter.\n");
  } else {
    printf("a is not a letter.\n");
  }
}

/**
 * Constructs a C-string, character by character, and prints out the full
 * string "Hello".
 */
void eight() {
  char *s = (char*)calloc(5,sizeof(char));

  s[0] = 'H';
  s[1] = 'e';
  s[2] = 'l';
  s[3] = 'l';
  s[4] = 'o';
  s[5] = '\0';
  printf("%s\n", s);

  free(s);
}

/**
 * Assigns a pointer (float *p) a numeric value (12.5).
 */
void nine() {
  float *p;
  float a = 12.5;
  p = &a;

  printf("The value of p is: %f\n", *p);
}

/**
 * Reset the value of x to zero.
 *
 * @param x
 *     Pointer to reset to 0.
 */
void ten(int *x) {
  *x = 0; 
}

/**
 * Concatenates "Hello " and the parameter str, which is guaranteed to be a
 * valid c string, and
 * prints the concatenated string.
 */
void eleven(const char *str) {
  char *s = "Hello ";
  char *result = (char*) calloc (13, sizeof(char));
  strcat(result, s);
  strcat(result, str);
  printf("%s\n", result);
  free(result);
}

/**
 * Creates an array of values containing the values {0.0, 0.1, ..., 0.9}.
 */
void twelve() {
  float *values = (float*) calloc (10, sizeof(float));

  int i, n = 10;
  for (i = 0; i < n; i++)
    values[i] = (float)i / n;

  for (i = 0; i < n; i++)
    printf("%f ", values[i]);
  printf("\n");
  free (values);
}

/**
 * Creates a 2D array of values and prints out the values on the diagonal.
 */
void thirteen(int a) {
  int **values;

  int i, j;
  values = (int**)calloc(10, sizeof(int*));
  for (i = 0; i < 10; i++){
    values[i] = (int*) calloc (10, sizeof(int));
    for (j = 0; j < 10; j++){
      values[i][j] = i * j * a;
    }
  }

  for (i = 0; i < 10; i++)
    printf("%d ", values[i][i]);
  printf("\n");
  
  for (i = 0; i< 10; i++){
    free(values[i]);
  }
  
  free (values);
}

/**
 * Prints out a specific string based on the input parameter (s).
 *
 * @param s
 *     Input parameter, used to determine which string is printed.
 */
void fourteen(const char *s) {
  int num = 0;
  if (strcmp(s, "blue") == 0){
    num = 1;
  } else if (strcmp(s, "orange") ==0){
    num = 2;
  }


  switch (num) {
  case 1:
    printf("Orange and BLUE!\n");
    break;

  case 2:
    printf("ORANGE and blue!\n");
    break;

  default:
    printf("orange and blue!\n");
    break;
  }
}


/**
 * Prints out a specific string based on the input parameter (value).
 *
 * @param value
 *     Input parameter, used to determine which string is printed.
 */
void fifteen(const int value) {
  switch (value) {
  case 1:
    printf("You passed in the value of one!\n");
    break;
  case 2:
    printf("You passed in the value of two!\n");
    break;
  default:
    printf("You passed in some other value!\n");
    break;
  }
}

/**
 * Returns a newly allocated string on the heap with the value of "Hello".
 * This should not be freed.
 *
 * @returns
 *     A newly allocated string, stored on the heap, with the value "Hello".
 */
char *sixteen() {
  char *s = (char*)calloc(5,sizeof(char));
  strcpy(s, "Hello");
  return s;
}

/**
 * Prints out the radius of a circle, given its diameter.
 *
 * @param d
 *     The diameter of the circle.
 */
void seventeen(const int d) {
  float a = d;
  printf("The radius of the circle is: %f.\n", a / 2);
}

/**
 * Manipulates the input parameter (k) and prints the result.
 *
 * @param k
 *     The input parameter to manipulate.
 */
void eighteen(const int k) {
  int a = k;
  a = a * a;
  a += a;
  a *= a;
  a = a-1;

  printf("Result: %d\n", a);
}


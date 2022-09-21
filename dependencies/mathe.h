#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

int Klammern(char text[], char saveto[]);
long double rechnen(char text[6]);
double hoch(double basis, int exponent);
int max(int arr[], int len);
int min(int arr[], int len);
double Random(int MIN, int MAX, int Komma);

#ifdef __cplusplus
}
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
    int num1, num2, num3;
    // Ask the user to type 3 numbers
    printf("Enter the length of the first side of the triangle: \n");
    // Get and save the number the user types
    scanf("%d", &num1);
    printf("Enter the length of the second side of the triangle: \n");
    scanf("%d", &num2);
    printf("Enter the hypotenuse length: \n");
    scanf("%d", &num3);

    if((pow(num1, 2) + pow(num2, 2)) == (pow(num3, 2))){
        double alpha = asin((double)num1/num3); 
        double beta = asin((double)num2/num3);
        double gama = asin((double)num3/num3);
        printf("The size of the angles in your triangle are:%f, %f, %f \n", 
        alpha, beta, gama);
    }
    else {
        printf("Error\n");
        exit(-1);
    }
    return 0;
}
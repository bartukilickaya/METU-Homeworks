#include <stdio.h>  
int main()  
{   // initialize the local variables.  
    int l, b, ar, pr;  
    printf("Enter the length of the rectangle");  
    scanf("%d", l); // take input from the user  
    printf("Enter the breadth of the rectangle");  
    scanf("%d", b);   
    ar = l * b; // calculate the area of rectangle.  
    pr = 2 * (l + b); // calculate the perimeter of rectangle.  
    printf("\n Area of Rectangle is: %d", ar);  
    printf("\n Perimeter of Rectangle is: %d", pr);    
}  
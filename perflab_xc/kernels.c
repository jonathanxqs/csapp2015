/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/
/*
  Name:    Xu's  perf lab
  Copyright:   
  Author:  Jonathan Xu
  Date: 15/05/15 19:32
  Description: 
*/


#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "12307130160",              /* Team name */
    "12307130160",     /* First member full name */
    "xucanxucan123@163.com",  /* First member email address */
    
    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst){
    int i, j;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}
/*
    My method :  unset the loop and rewrite by s&l directly
    CPE :  effectively decreased after relaxing

*/
char rotate_descr[] = "rotate: Using 2 pointer";
void rotate(int dim, pixel *src, pixel *dst){
    int i, j;
    int dst_base = (dim-1)*dim;
    dst += dst_base;
    for (i = 0; i < dim; i+=32){ 
            for (j = 0; j < dim; j++){ 
                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;
                src+=dim;
                dst+=1;

                *dst=*src;

                    src++;
                    src -= (dim<<5)-dim;
                    dst-=31+dim;
                }
            dst+=dst_base+dim;
            dst+=32;
            src += 31*dim;

        }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions(){
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}

/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct{
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

typedef struct{
    int red;
    int green;
    int blue;
} pixel_int;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum){
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* static void setPixelLeft(int pos,int dim,pixel *src,pixel *dst) {
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p){
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum){
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src){
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;
     int maxI = max(i-1, 0);
     int maxJ = max(j-1, 0);
     int minI = min(i+1, dim-1);
     int minJ = min(j+1, dim-1);

    initialize_pixel_sum(&sum);
    for(ii = maxI; ii <= minI; ii++)
    for(jj = maxJ; jj <= minJ; jj++)
        accumulate_sum(&sum, src[ii*dim+jj]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst){
    int i, j;

    for (i = 0; i < dim; i++) 
      {
        for (j = 0; j < dim; j++)
          {
           dst[i*dim+j] = avg(dim, i, j, src);
          }
     }
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */

/*
    My method :  unset the loop and rewrite by s&l directly
    CPE :  effectively decreased after relaxing

*/
static void setEdges(int dim, pixel *src, pixel *dst){
    pixel_int cache;
    pixel result;

    // TOPLEFT
    cache.red = cache.green = cache.blue = 0;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src++;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src--;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src--;

    result.red   = cache.red>>2;
    result.green = cache.green>>2;
    result.blue  = cache.blue>>2;
    *dst = result;
    dst += dim-1;

    // top-right
    cache.red = cache.green = cache.blue = 0; 
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src--;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src++;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=1+dim*(dim-3);

    result.red   = cache.red>>2;
    result.green = cache.green>>2;
    result.blue  = cache.blue>>2;
    *dst = result;
    dst+=1+dim*(dim-2);

    // bottom-left
    cache.red = cache.green = cache.blue = 0;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src++;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src-=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src--;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=dim-1;

    result.red   = cache.red>>2;
    result.green = cache.green>>2;
    result.blue  = cache.blue>>2;
    *dst = result;
    dst+=dim-1;

    // bottom-right
    cache.red = cache.green = cache.blue = 0;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src--;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src++;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;

    result.red   = cache.red>>2;
    result.green = cache.green>>2;
    result.blue  = cache.blue>>2;
    *dst = result;
}

static void setPixelTop(int pos,int dim,pixel *src,pixel *dst){
    pixel_int cache;
    pixel result;
    cache.red = cache.green = cache.blue = 0;

    src += pos;
    dst += pos;

    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src--;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src++;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src++;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src-=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;

    result.red   = cache.red/6;
    result.green = cache.green/6;
    result.blue  = cache.blue/6;
    *dst = result;  
}

static void setPixelLeft(int pos,int dim,pixel *src,pixel *dst){
    pixel_int cache;
    pixel result;
    cache.red = cache.green = cache.blue = 0;

    src += pos;
    dst += pos;

    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src-=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src++;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src--;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;

    result.red   = cache.red/6;
    result.green = cache.green/6;
    result.blue  = cache.blue/6;
    *dst = result;  
}

static void setPixelBottom(int pos,int dim,pixel *src,pixel *dst){
    pixel_int cache;
    pixel result;
    cache.red = cache.green = cache.blue = 0; 

    src += pos;
    dst += pos;

    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src++;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src-=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src--;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src--;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;

    result.red   = cache.red/6;
    result.green = cache.green/6;
    result.blue  = cache.blue/6;
    *dst = result;
}

static void setPixelRight(int pos,int dim,pixel *src,pixel *dst){
    pixel_int cache;
    pixel result;
    cache.red = cache.green = cache.blue = 0;

    src += pos;
    dst += pos;

    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src-=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src--;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src+=dim;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;
    src++;
    cache.red   += src->red;
    cache.green += src->green;
    cache.blue  += src->blue;

    result.red   = cache.red/6;
    result.green = cache.green/6;
    result.blue  = cache.blue/6;
    *dst = result;
}

char smooth_descr[] = "smooth: Developing new algo";
void smooth(int dim, pixel *src, pixel *dst){
    int i,j;
    int temp;

    temp = dim*dim-dim;

    setEdges(dim,src,dst);

    for(j = 1;j < dim-1;j++){
        setPixelTop(j,dim,src,dst);
        setPixelBottom((dim-1)*dim+j,dim,src,dst);
    }

    for(i = 1;i < dim-1;i++){
        setPixelLeft(i*dim,dim,src,dst);
        setPixelRight(i*dim+dim-1,dim,src,dst);
    }

    temp = dim;

    pixel_int tleft;
    pixel_int tright;
    pixel_int bleft;
    pixel_int bright;
    dst+=1+dim;
    src+=1+dim;

    for(i = 1;i < dim-1; i+=2){  
        for(j = 1;j < dim-1;j+=2){

            tleft.red = src->red;
            tleft.green = src->green;
            tleft.blue = src->blue;
            src++;
            tleft.red += src->red;
            tleft.green += src->green;
            tleft.blue += src->blue;
            src+=dim;
            tleft.red += src->red;
            tleft.green += src->green;
            tleft.blue += src->blue;
            src--;
            tleft.red += src->red;
            tleft.green += src->green;
            tleft.blue += src->blue;
            src--;
            tright = bleft = bright = tleft;
            tleft.red += src->red;
            tleft.green += src->green;
            tleft.blue += src->blue;
            src-=dim;
            tleft.red += src->red;
            tleft.green += src->green;
            tleft.blue += src->blue;
            bleft = tleft;
            src-=dim;
            tleft.red += src->red;
            tleft.green += src->green;
            tleft.blue += src->blue;
            src++;
            tleft.red += src->red;
            tleft.green += src->green;
            tleft.blue += src->blue;
            tright.red += src->red;
            tright.green += src->green;
            tright.blue += src->blue;
            src++;
            tleft.red += src->red;
            tleft.green += src->green;
            tleft.blue += src->blue;
            tright.red += src->red;
            tright.green += src->green;
            tright.blue += src->blue;
            src++;  
            tright.red += src->red;
            tright.green += src->green;
            tright.blue += src->blue;
            src+=dim;
            bright.red += src->red;
            bright.green += src->green;
            bright.blue += src->blue;
            tright.red += src->red;
            tright.green += src->green;
            tright.blue += src->blue;
            src+=dim;
            bright.red += src->red;
            bright.green += src->green;
            bright.blue += src->blue;
            tright.red += src->red;
            tright.green += src->green;
            tright.blue += src->blue;
            src+=dim;
            bright.red += src->red;
            bright.green += src->green;
            bright.blue += src->blue;
            src--;
            bright.red += src->red;
            bright.green += src->green;
            bright.blue += src->blue; 
            bleft.red += src->red;
            bleft.green += src->green;
            bleft.blue += src->blue;
            src--;
            bright.red += src->red;
            bright.green += src->green;
            bright.blue += src->blue;
            bleft.red += src->red;
            bleft.green += src->green;
            bleft.blue += src->blue;
            src--;
            bleft.red += src->red;
            bleft.green += src->green;
            bleft.blue += src->blue;
            src-=2*dim-3;

            dst->red   = tleft.red/9;
            dst->green = tleft.green/9;
            dst->blue  = tleft.blue/9; 
            dst+=dim;
            dst->red   = bleft.red/9;
            dst->green = bleft.green/9;
            dst->blue  = bleft.blue/9; 
            dst++;
            dst->red   = bright.red/9;
            dst->green = bright.green/9;
            dst->blue  = bright.blue/9; 
            dst-=dim;
            dst->red   = tright.red/9;
            dst->green = tright.green/9;
            dst->blue  = tright.blue/9; 
            dst++;
            }

        src+=2+dim;
        dst+=2+dim;
    }

}

/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions(){
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}
/*
          Jonathan Xu    12307130160
*/

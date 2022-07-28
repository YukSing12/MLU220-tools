#include <arm_neon.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void rgb_deinterleave_c(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *rgb, int len_color) {
    /*
     * Take the elements of "rgb" and store the individual colors "r", "g", and "b".
     */
    for (int i=0; i < len_color; i++) {
        r[i] = rgb[3*i];
        g[i] = rgb[3*i+1];
        b[i] = rgb[3*i+2];
    }
}

void rgb_deinterleave_neon(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *rgb, int len_color) {
    /*
     * Take the elements of "rgb" and store the individual colors "r", "g", and "b"
     */
    int num8x16 = len_color / 16;
    uint8x16x3_t intlv_rgb;
    for (int i=0; i < num8x16; i++) {
        intlv_rgb = vld3q_u8(rgb+3*16*i);
        vst1q_u8(r+16*i, intlv_rgb.val[0]);
        vst1q_u8(g+16*i, intlv_rgb.val[1]);
        vst1q_u8(b+16*i, intlv_rgb.val[2]);
    }
}

int main(int argc, char** argv)
{
	const int img_h = 640;
	const int img_w = 640;
	const int img_c = 3;
	const int len = img_h * img_w * img_c;
	uint8_t *rgb = (uint8_t*) malloc(img_h * img_w * img_c);
	uint8_t *r   = (uint8_t*) malloc(img_h * img_w);
	uint8_t *g   = (uint8_t*) malloc(img_h * img_w);
	uint8_t *b   = (uint8_t*) malloc(img_h * img_w);
	int i;
	double t;
	clock_t start, end;
	
	printf("Allocated memory.\n");
	for(i = 0; i < img_h * img_w; ++i)
	{
		*(rgb + i + 0) = 255;
		*(rgb + i + 1) = 182;
		*(rgb + i + 2) = 193;
	}
	printf("Initialized rgb.\n");
	start = clock();
	rgb_deinterleave_c(r, g, b, rgb, img_h * img_w);
	end = clock();
	t = (double)(end-start)/CLOCKS_PER_SEC;
		
	printf("C cost %f \n",t);
	
	start = clock();
	rgb_deinterleave_neon(r, g, b, rgb, img_h * img_w);
	end = clock();
	t = (double)(end-start)/CLOCKS_PER_SEC;
		
	printf("NEON C cost %f \n",t);

	free(rgb);	
	free(r);
	free(g);
	free(b);
	return 0;
}

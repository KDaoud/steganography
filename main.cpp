#include "Image.h"

int main (int argc, char** argv)
{
    Image test("./testimg/test.jpg");
    
    /*
    test.write ("./test/new.png");

    Image copy = test;
    for (int i = 0; i < copy.w*copy.channels;++i)
    {
            copy.data[i] = 255;
    }

    copy.write("./test/copy.png");
    */

    test.encodeMsg("This is a top secret message hidden in an image using steganography");
    test.write("./test/encodedTest.png");

    char buffer[200] = {0};
    test.decodeMsg(buffer);

    printf("message:\n");
    printf("%s\n", buffer);

    return 0;
}
#ifndef BLOB_H_
#define BLOB_H_

//Blob template.
struct blob_t {
    float x;        //X location of the blob.
    float y;        //Y location of the blob.
    float weight;   //Weighting, based on pixel group.
    int id;         //Blob ID.
    bool valid;     //Whether a blob is valid.
};

#endif

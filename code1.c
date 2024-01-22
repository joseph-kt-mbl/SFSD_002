void CREATE_HEADER(int FIRSTB, int LASTB, int FREE_POSITION_B,int DELETED){
    
    FILE * verify =fopen(fileName,"rb");
    header h;
    if(verify!=NULL){
        fclose(verify);
        return;// return if the header exist.
    }
    FILE * w =fopen(fileName,"wb");
    

    header H;
    H.FIRSTB=FIRSTB;
    H.LASTB=LASTB;
    H.FREE_POSITION_B=FREE_POSITION_B;
    H.DELETED=DELETED;
    
    fwrite(&H,sizeof(H),1,w);
    
    fclose(w);
}
int HEADER(int INDEX){
    
    FILE * f =fopen(fileName,"rb");
    header H;
    
    if (f== NULL) {
        printf("Unable to open the file.\n");
        fclose(f);
        return -111;        //value of null
    }
    fread(&H, sizeof(H), 1, f);    
        
    switch(INDEX){
        case 1: 
                fclose(f);
                return H.FIRSTB;
                break;
        case 2: 
                fclose(f);
                return H.LASTB;
                break;
                
        case 3:
                fclose(f);
                return H.FREE_POSITION_B;
                break;
        case 4:
                fclose(f);
                return H.DELETED;
                break;
        default:
            fclose(f);
            return -222;    //INDEX OverFlow Value
    }
    fclose(f);
}
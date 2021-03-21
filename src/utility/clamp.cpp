void clamp(float &variable, float maxpoint){
    variable = (variable>0&&variable>maxpoint) ? maxpoint : variable;
    variable = (variable<0&&variable<-maxpoint) ? -maxpoint : variable;
}
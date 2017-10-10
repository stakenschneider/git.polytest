function cl=classe(x,y)
    cl=0;
    %1
    if(  (x-4)^2 + (y-12)^2<=9)
          if(  (x-4)^2 + (y-12)^2>=1)
                cl=1;
          end
    end
    
    %2
    if(x>=10 & x<=14 & y>=10 & y<=14)
        cl=2;
    end
    
    %3
    xn=(y+26)/2;
    xn2=(y+38)/2;
    if(xn<=x & x<=xn2 & 10<=y && y<=14)
        cl=3;
    end
    
    %4
    xn=(y+44)/2;
    xn2=(y-78)/-2;
    if(xn<=x & x<=xn2 & 10<=y && y<=14)
        cl=4;
    end
    
    %5
    xn=(y+2)/2;
    xn2=(y-18)/-2;
    if(xn<=x & x<=xn2 & 4<=y && y<=8)
        cl=5;
    end
    
    %6
    xn=(y-28)/-2;
    xn2=(y+24)/2;
    if(xn<=x & x<=xn2 & 4<=y && y<=8)
        cl=6;
    end
    if(  (x-15)^2 + (y-6)^2<=1)
        cl=6;
    end
    
    %7
    if(  (x-21)^2 + (y-6)^2<=9)
          if(  (x-21)^2 + (y-6)^2>=4)
              if(x<=21)
                cl=7;
              end
          end
    end
    
    %8
     if(x>=5 & x<=26 & y>=4 & y<=8)
        cl=8;
     end
    
     %9
      if(  (x-31)^2 + (y-4)^2<=9)
           cl=9;
      end
      xn=(y+38/3)*3/2;
      xn2=(y+38/3)*3/-2;
      if(xn<=x & x<=xn2 & 6<=y && y<=8)
          cl=9;
      end
end
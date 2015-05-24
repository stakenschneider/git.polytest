 Uses CRT;
var
life:array[1..100,1..100] of integer;
s:string;
x,y,l,i,j,n:integer;
begin

ClrScr;
for x:=1 to 26 do
for y:=1 to 26 do
life[x,y]:=0;


ReadLn();
ClrScr;
x:=50;
y:=50;
while (x<>0) or (y<>0)   do begin
life[x,y]:=1;
Write('введите  x : ');
Read(x);
if x<>0 then
x:=x+1;
 Write('введите  y : ');
Read(y);
if y<>0 then
y:=y+1;
end;
ClrScr;
n:=1;
textcolor(2);
for x:=2 to 25 do begin
for y:=2 to 25 do
if life[x,y]=1 then
write(#254)
else
write(#255);
writeln;
end;
while n<>0 do begin
n:=0;
for x:=2 to 25 do
for y:=2 to 25 do begin
L:=0;
if (life[x-1,y]=1) or (life[x-1,y]=2) then
l:=L+1;
if (life[x-1,y+1]=1) or (life[x-1,y+1]=2) then
l:=l+1;
if (life[x,y+1]=1) or (life[x,y+1]=2) then
l:=l+1;
if (life[x+1,y+1]=1) or (life[x+1,y+1]=2) then
l:=l+1;
if (life[x+1,y]=1) or (life[x+1,y]=2) then
l:=l+1;
if (life[x+1,y-1]=1) or (life[x+1,y-1]=2) then
l:=l+1;
if (life[x,y-1]=1) or (life[x,y-1]=2) then
l:=l+1;
if (life[x-1,y-1]=1) or (life[x-1,y-1]=2) then
l:=l+1;
if ((l>3) or (l<2)) and (life[x,y]=1) then
life[x,y]:=2;
L:=0;
end;
 
 
for x:=2 to 25 do
for y:=2 to 25 do begin
L:=0;
if (life[x-1,y]=1) or (life[x-1,y]=2)  then
l:=L+1;
if (life[x-1,y+1]=1) or (life[x-1,y+1]=2) then
l:=l+1;
if (life[x,y+1]=1) or (life[x,y+1]=2) then
l:=l+1;
if (life[x+1,y+1]=1) or (life[x+1,y+1]=2) then
l:=l+1;
if (life[x+1,y]=1) or (life[x+1,y]=2) then
l:=l+1;
if (life[x+1,y-1]=1) or (life[x+1,y-1]=2) then
l:=l+1;
if (life[x,y-1]=1) or (life[x,y-1]=2) then
l:=l+1;
if (life[x-1,y-1]=1) or (life[x-1,y-1]=2) then
l:=l+1;
if (l=3) and (life[x,y]<>1) then
life[x,y]:=3;
L:=0;
end;
for x:=2 to 25 do
for y:=2 to 25 do begin
if life[x,y]=2 then
life[x,y]:=0;
end;
for x:=2 to 25 do
for y:=2 to 25 do begin
if life[x,y]=3 then
life[x,y]:=1;
end;
delay(1000);
ClrScr;
for x:=2 to 25 do begin
for y:=2 to 25 do
if life[x,y]=1 then
write(#254)
else
write(#255);
writeln;
end;
n:=0;
for x:=2 to 25 do begin
for y:=2 to 25 do
if life[x,y]=1 then
n:=n+1;
end;
write('kol-vo shivih lketok ',N);
end;
ClrScr;
WriteLn('Life End');
ReadLn(i);
end.

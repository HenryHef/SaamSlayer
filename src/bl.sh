
rm -r /home/henry/workspace/SWIG_BUILD/
mkdir /home/henry/workspace/SWIG_BUILD/

cp -a . /home/henry/workspace/SWIG_BUILD/
echo "CP DONE"
cd /home/henry/workspace/SWIG_BUILD/
#in ~/workspace/SWIG_BUILD

swig -c++ -python -py3 *.i
echo "SWIG DONE"
g++ -std=c++1y -O3 -fPIC -c *.cpp *.h
echo "MAIN COMPILE DONE"
g++ -std=c++1y -O3 -fPIC -c *_wrap.cxx -I/usr/include/python3.5
echo "SWIG COMPILE DONE"
g++ -shared *.o -o _chess_engin.so
touch "__init__.py"
#in ~/workspace/SWIG_BUILD

cp _chess_engin.so /home/henry/workspace/CELib/_chess_engin.so
cp chess_engin.py /home/henry/workspace//CELib/chess_engin.py

cd -
echo "DONE"

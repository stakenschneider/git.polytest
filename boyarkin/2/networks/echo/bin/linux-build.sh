#!/bin/bash

set -e

# ----------  Make client executable  ---------

echo "Try to compile client files."
mkdir -p networks-build
cp -R client/source/ ./networks-build

# Delete annotations.
find networks-build/source -name "*.java" -exec sed -i 's/@Nullable//g' {} \;
find networks-build/source -name "*.java" -exec sed -i 's/@NotNull//g' {} \;
find networks-build/source -name "*.java" -exec sed -i 's/import com.sun.istack.internal.NotNull;//g' {} \;
find networks-build/source -name "*.java" -exec sed -i 's/import com.sun.istack.internal.Nullable;//g' {} \;
find networks-build/source -name "*.java" > networks-build/sources.txt

# Compile classes.
javac -d ./networks-build/source @./networks-build/sources.txt
cd ./networks-build/source/

# Remove excess files.
find . -name "*.java" -type f -delete
find . -name "*.MF" -type f -delete
find . -name "META-INF" -type d -delete

# Compile jar
jar cvfe ../../bin/client/jar/client.jar Main *.class > /dev/null
echo "Success. Executable path: ./bin/client/jar/client.jar"

# Rollback
cd ../..
rm -r ./networks-build/source/
rm -r ./networks-build/sources.txt



# ----------  Make server executable  ---------

echo "Try to compile server files."
cp -R server/source/ ./networks-build

# Delete annotations.
find networks-build/source -name "*.java" -exec sed -i 's/@Nullable//g' {} \;
find networks-build/source -name "*.java" -exec sed -i 's/@NotNull//g' {} \;
find networks-build/source -name "*.java" -exec sed -i 's/import com.sun.istack.internal.NotNull;//g' {} \;
find networks-build/source -name "*.java" -exec sed -i 's/import com.sun.istack.internal.Nullable;//g' {} \;
find networks-build/source -name "*.java" > networks-build/sources.txt

# Compile classes.
javac -d ./networks-build/source @./networks-build/sources.txt
cd ./networks-build/source/

# Remove excess files.
find . -name "*.java" -type f -delete
find . -name "*.MF" -type f -delete
find . -name "META-INF" -type d -delete

# Compile jar
jar cvfe ../../bin/server/jar/server.jar Main *.class > /dev/null
echo "Success. Executable path: ./bin/server/jar/server.jar"

# Rollback
cd ../..
rm -r ./networks-build




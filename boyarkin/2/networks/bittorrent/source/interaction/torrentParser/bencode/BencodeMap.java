/*
    Copyright 2008, 2009 Wolfgang Ginolas 
 
    This file is part of P2PVPN. 
 
    P2PVPN is free software: you can redistribute it and/or modify 
    it under the terms of the GNU Lesser General Public License as published by 
    the Free Software Foundation, either version 3 of the License, or 
    (at your option) any later version. 
 
    Foobar is distributed in the hope that it will be useful, 
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    GNU Lesser General Public License for more details. 
 
    You should have received a copy of the GNU Lesser General Public License 
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>. 
*/

package interaction.torrentParser.bencode;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.HashMap;

public class BencodeMap extends HashMap<BencodeString, BencodeObject> implements BencodeObject {

    public BencodeMap() {
        super();
    }

    public void write(OutputStream out) throws IOException {
        out.write('d');

        Object[] keys = this.keySet().toArray();
        Arrays.sort(keys);
        for (Object key: keys) {
            ((BencodeString) key).write(out);
            this.get(key).write(out);
        }

        out.write('e');
    }

    @Override
    public synchronized String toString() {
        StringBuilder result = new StringBuilder();
        boolean first = true;

        result.append('(');

        Object[] keys = this.keySet().toArray();
        Arrays.sort(keys);
        for (Object key: keys) {
            if (!first)
                result.append(", ");
            result.append(((BencodeString) key).toString()).append(":").append(this.get(key).toString());
            first = false;
        }

        result.append(')');
        return result.toString();
    }
}
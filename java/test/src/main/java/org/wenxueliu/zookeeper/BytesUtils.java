package org.wenxueliu.zookeeper;

import java.nio.charset.StandardCharsets;

public class BytesUtils {

    static public String bytesToString(byte[] bytes) {
        int len = bytes.length;
        if (bytes == null || len == 0) {
            throw new IllegalArgumentException("bytes cannot be empty");
        }
        //StringBuilder builder = new StringBuilder(len * 5);
        //for (int i = 0; i < len; i++) {
        //    builder.append(bytes[i]);
        //}
        //return builder.toString();

        char[] chars = new char[len];
        for (int i = 0; i < len; i++) {
            chars[i] = (char)(bytes[i] & 0xff);
        }
        return new String(chars);

        //return new String(bytes, StandardCharsets.UTF_8);

        //return new String(bytes, 0);
    }

    static public byte[] StringToBytes(String str) {
        if (str == null) {
            throw new IllegalArgumentException("str cannot be empty");
        }

        //byte[] dstBytes = new byte[str.length()];
        //str.getBytes(0, str.length(), dstBytes, 0);
        //return dstBytes;

        //return str.getBytes(StandardCharsets.UTF_8);

        int len = str.length();
        byte[] bytes = new byte[len];
        char[] chars = str.toCharArray();
        for (int i = 0; i < len; i++) {
            bytes[i] = (byte)chars[i];
        }
        return bytes;
    }
}

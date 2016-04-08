package org.wenxueliu.zookeeper;

import java.nio.charset.StandardCharsets;
import java.nio.ByteBuffer;
import java.nio.ByteBuffer;
import java.io.IOException;

import org.apache.commons.codec.net.QuotedPrintableCodec;
import org.apache.commons.codec.DecoderException;

import org.apache.commons.codec.binary.BaseNCodec;
import org.apache.commons.codec.binary.Base32;

public class BytesUtils {

    static final String MATIC_STRING = "-";

    static public String bytesToString(byte[] bytes) {
        int len = bytes.length;
        if (bytes == null || len == 0) {
            throw new IllegalArgumentException("bytes cannot be empty");
        }
        //StringBuilder builder = new StringBuilder(len * 5);
        //for (int i = 0; i < len-1; i++) {
        //    builder.append(bytes[i]).append(",");
        //}
        //builder.append(bytes[len-1]);
        //return builder.toString();

        //char[] chars = new char[len];
        //for (int i = 0; i < len; i++) {
        //    chars[i] = (char)(bytes[i] & 0xff);
        //}
        //return new String(chars);

        //return new String(bytes, StandardCharsets.UTF_8);

        //return new String(bytes, StandardCharsets.US_ASCII);

        //return new String(bytes, StandardCharsets.ISO_8859_1);

        //return new String(bytes, 0);


        //byte[] tmpBytes = new QuotedPrintableCodec().encode(bytes);
        //char[] chars = new char[len];
        //for (int i = 0; i < len; i++) {
        //    chars[i] = (char)(tmpBytes[i] & 0xff);
        //}
        //return new String(chars);
        return new Base32().encodeAsString(bytes);
    }

    static public byte[] StringToBytes(String str) {
        if (str == null) {
            throw new IllegalArgumentException("str cannot be empty");
        }

        //ByteBuffer outBytes = ByteBuffer.allocate(str.length());
        //for(String s : str.split(",")) {
        //    outBytes.put(Byte.parseByte(s));
        //}
        //return outBytes.array();

        //byte[] dstBytes = new byte[str.length()];
        //str.getBytes(0, str.length(), dstBytes, 0);
        //return dstBytes;

        //return str.getBytes(StandardCharsets.US_ASCII);
        //return str.getBytes(StandardCharsets.UTF_8);
        //return str.getBytes(StandardCharsets.ISO_8859_1);

        //int len = str.length();
        //byte[] bytes = new byte[len];
        //char[] chars = str.toCharArray();
        //for (int i = 0; i < len; i++) {
        //    bytes[i] = (byte)chars[i];
        //}
        ////return bytes;
        //try {
        //    return new QuotedPrintableCodec().decode(bytes);
        //} catch (DecoderException e) {
        //    System.out.println("StringToBytes error: " + e.getMessage());
        //    return null;
        //}

        return new Base32().decode(str);
    }
}

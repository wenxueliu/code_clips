package org.wenxueliu.pdfbox;

import java.io.FileInputStream;
import org.apache.pdfbox.cos.COSDocument;
import org.apache.pdfbox.pdfparser.PDFParser;
import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.pdfbox.util.PDFTextStripper;

public class PDFEditor {
    public String readFdf(String file) {
        String docText = "";
        PDDocument doc = null;
        try {
            FileInputStream fis = new FileInputStream(file);
            COSDocument cosDoc = null;
            PDFParser parser = new PDFParser(fis);
            parser.parse();
            cosDoc = parser.getDocument();
            PDFTextStripper stripper = new PDFTextStripper();
            stripper.setStartPage(1);
            stripper.setEndPage(1);
            doc = new PDDocument(cosDoc);
            docText = stripper.getText(new PDDocument(cosDoc));
            //System.out.println("begin page:", stripper.getStartPage());
            //System.out.println("end page:", stripper.getEndPage());
            System.out.println(docText);
        } catch (Exception e) {
                System.out.println("bb=" + e.getMessage());
        } finally {
        }
        return docText;
    }
    public static void main(String args[]) {
        PDFEditor read = new PDFEditor();
        String s = read.readFdf("/home/wenxueliu/book/release-notes.en.pdf");
        System.out.println(s);
    }
}

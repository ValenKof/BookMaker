import java.io.*;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.util.*;

public class FileIO {

    public static String readAll(File file) throws IOException {
        byte[] encoded = Files.readAllBytes(file.toPath());
        return new String(encoded, Charset.defaultCharset());
    }

    public static void writeAll(String text, File file) throws IOException {
        try (PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(file)))) {
            out.print(text);
            out.flush();
        }
    }

    public static List<File> getFiles(List<File> files, String extension) {
        List<File> result = new ArrayList<>();
        for (File file : files) {
            if (file.getName().endsWith(extension)) {
                result.add(file);
            }
        }
        return result;
    }

    public static List<File> listFiles(File file) {
        List<File> list = new ArrayList<>();
        File[] files = file.listFiles();
        if (files != null) {
            list.addAll(Arrays.asList(files));
        }
        return list;
    }

}

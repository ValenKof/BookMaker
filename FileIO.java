import java.io.*;
import java.util.*;

public class FileIO {

    public static String readAll(File file) throws IOException {
        StringBuilder builder = new StringBuilder();
        try (Scanner in = new Scanner(new BufferedReader(new FileReader(file)))) {
            while (in.hasNextLine()) {
                builder.append(in.nextLine()).append('\n');
            }
        }
        return builder.toString();
    }

    public static void writeAll(String text, File file) throws IOException {
        try (PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(file)))) {
            out.print(text);
            out.flush();
        }
    }

    public static List<File> getFiles(File[] files, String extension) {
        if (files == null) {
            return Collections.emptyList();
        }
        List<File> result = new ArrayList<>();
        for (File file : files) {
            if (file.getName().endsWith(extension)) {
                result.add(file);
            }
        }
        return result;
    }

}

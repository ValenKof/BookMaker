import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class SourceEditor {

    public static void replaceTabs(File file, int nSpaces) throws IOException {
        FileIO.writeAll(replaceTabs(FileIO.readAll(file), nSpaces), file);
    }

    public static String replaceTabs(String text, int nSpaces) {
        StringBuilder replace = new StringBuilder();
        for (int i = 0; i < nSpaces; ++i) {
            replace.append(' ');
        }
        return text.replaceAll("\t", replace.toString());
    }

    public static List<Integer> checkWidth(File file, int width) throws IOException {
        return checkWidth(FileIO.readAll(file), width);
    }

    public static List<Integer> checkWidth(String text, int width) {
        List<Integer> result = new ArrayList<>();
        String[] lines = text.split("\n");
        for (int iLine = 0; iLine < lines.length; ++iLine) {
            if (lines[iLine].length() > width) {
                result.add(iLine + 1);
            }
        }
        return result;
    }

    public static String removeNewLines(String text) {
        int l = 0;
        while (l < text.length() && text.charAt(l) == '\n') {
            l++;
        }
        int r = text.length();
        while (r > 0 && text.charAt(r - 1) == '\n') {
            r--;
        }
        return l < r ? text.substring(l, r) : "";
    }

    public static void removeNewLines(File file) throws IOException {
        FileIO.writeAll(removeNewLines(FileIO.readAll(file)), file);
    }

    public static String removeSpaces(String text) {
        String[] lines = text.split("\n");
        StringBuilder builder = new StringBuilder();
        for (int iLine = 0; iLine < lines.length; ++iLine) {
            if (iLine != 0) {
                builder.append('\n');
            }
            int size = lines[iLine].length();
            while (size > 0 && Character.isWhitespace(lines[iLine].charAt(size - 1))) {
                size--;
            }
            builder.append(lines[iLine].substring(0, size));
        }
        return builder.toString();
    }

    public static void removeSpaces(File file) throws IOException {
        FileIO.writeAll(removeSpaces(FileIO.readAll(file)), file);
    }
}

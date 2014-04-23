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

}

import java.io.*;
import java.util.*;
import java.util.concurrent.*;

public class BookMaker {

    public static final int THREADS_NUMBER = 8;
    public static final int TABS_WIDTH = 2;
    public static final int LINES_WIDTH = 65;

    public static void REQUIRE(boolean condition, String message) {
        if (!condition) {
            System.err.println(message);
            System.exit(1);
        }
    }

    public static String makeHeader(List<File> files) {
        StringBuilder header = new StringBuilder();
        for (File file : files) {
            try {
                header.append(FileIO.readAll(file));
            } catch (IOException e) {
                REQUIRE(false, e.getMessage());
            }
        }
        return header.toString();
    }

    public static void main(String[] args) {

        REQUIRE(args.length == 1, "Usage: <dir>");

        File dir = new File(args[0]);
        REQUIRE(dir.isDirectory(), args[0] + " is not a dir");

        List<File> hFiles = FileIO.getFiles(dir.listFiles(), ".h");
        String header = makeHeader(hFiles);

        ExecutorService executor = Executors.newFixedThreadPool(THREADS_NUMBER);
        List<Callable<String>> tasks = new ArrayList<>();
        List<File> cppFiles = FileIO.getFiles(dir.listFiles(), ".cpp");
        for (File file : cppFiles) {
            tasks.add(new CppCompiler(file, header));
        }
        try {
            List<Future<String>> results = executor.invokeAll(tasks);
            for (Future<String> result : results) {
                REQUIRE(result.isDone(), "not done");
                REQUIRE(result.get().equals(CppCompiler.COMPILED), result.get());
            }
            executor.shutdown();
        } catch (InterruptedException | ExecutionException e) {
            REQUIRE(false, e.getMessage());
        }
    }
}

class CppCompiler implements Callable<String> {

    public static final String COMPILED = "Compiled";
    public static final String EXCEPTION = "Exception";
    public static final String LONG_LINES = "Long Lines";

    public static final String GCC = "g++ -g -O2 -std=gnu++0x -static";

    private final File file;
    private final String header;

    public CppCompiler(File file, String header) {
        this.file = file;
        this.header = header;
    }


    @Override
    public String call() {
        try {
            SourceEditor.replaceTabs(file, BookMaker.TABS_WIDTH);
            SourceEditor.removeNewLines(file);
            SourceEditor.removeSpaces(file);

            String code = FileIO.readAll(file);

            List<Integer> longLines = SourceEditor.checkWidth(code, BookMaker.LINES_WIDTH);
            if (!longLines.isEmpty()) {
                StringBuilder builder = new StringBuilder();
                builder.append(LONG_LINES).append(" at ").append(file.getName()).append(":");
                for (int iLine : longLines) {
                    builder.append(' ').append(iLine);
                }
                return builder.toString();
            }

            StringBuilder source = new StringBuilder();
            source.append(header).append('\n').append(code).append('\n');
            if (!code.contains("int main()")) {
                source.append("int main() { return 0; }");
            }

            File cppFile = File.createTempFile("_compile_", file.getName(), file.getParentFile());
            cppFile.deleteOnExit();

            PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(cppFile)));
            out.write(source.toString());
            out.flush();
            out.close();

            String exeFile = cppFile.getAbsolutePath().replace(".cpp", ".exe");
            new File(exeFile).deleteOnExit();
            Process compile = Runtime.getRuntime().exec(GCC + " " + cppFile.getAbsolutePath() + " -o " + exeFile);

            if (compile.waitFor() == 0) {
                return COMPILED;
            }

            Scanner err = new Scanner(compile.getErrorStream());
            StringBuilder message = new StringBuilder();
            message.append(file.getName()).append(": ").append('\n');
            while (err.hasNextLine()) {
                message.append(err.nextLine()).append('\n');
            }
            err.close();
            return message.toString();
        } catch (IOException | InterruptedException e) {
            return EXCEPTION + ": " + e.getMessage();
        }
    }
}

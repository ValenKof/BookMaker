import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.*;

public class BookMaker {

    public static final int THREADS_NUMBER = 8;
    public static final int TABS_WIDTH = 2;

    public static void require(boolean condition, String message) {
        if (!condition) {
            System.err.println(message);
            System.exit(1);
        }
    }

    public static String readAll(File file) {
        StringBuilder builder = new StringBuilder();
        try (Scanner in = new Scanner(new BufferedReader(new FileReader(file)))) {
            while (in.hasNextLine()) {
                builder.append(in.nextLine()).append('\n');
            }
            return builder.toString();
        } catch (IOException e) {
            BookMaker.require(false, e.getMessage());
        }
        return builder.toString();
    }

    public static void writeAll(String text, File file) {
        try (PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(file)))) {
            out.print(text);
            out.flush();
        } catch (IOException e) {
            BookMaker.require(false, e.getMessage());
        }
    }

    public static String makeHeader(List<File> files) {
        StringBuilder header = new StringBuilder();
        for (File file : files) {
            header.append(readAll(file)).append('\n');
        }
        return header.toString();
    }

    public static void replaceTabs(File file, int nSpaces) {
        StringBuilder replace = new StringBuilder();
        for (int i = 0; i < nSpaces; ++i) {
            replace.append(' ');
        }
        String text = readAll(file);
        text = text.replaceAll("\t", replace.toString());
        writeAll(text, file);
    }

    public static void main(String[] args) {

        require(args.length == 1, "Usage: <dir>");

        File dir = new File(args[0]);
        require(dir.isDirectory(), args[0] + " is not a dir");

        List<File> hFiles = new ArrayList<File>();
        List<File> cppFiles = new ArrayList<File>();

        File[] files = dir.listFiles();
        require(files != null, "Can't open dir");

        for (File file : files) {
            if (file.getName().endsWith(".h")) {
                hFiles.add(file);
            }
            if (file.getName().endsWith(".cpp")) {
                cppFiles.add(file);
            }
        }


        String header = makeHeader(hFiles);

        ExecutorService executor = Executors.newFixedThreadPool(THREADS_NUMBER);
        List<Callable<String>> tasks = new ArrayList<>();
        for (File file : cppFiles) {
            tasks.add(new CppCompiler(file, header));
        }
        List<Future<String>> results = Collections.emptyList();
        try {
            results = executor.invokeAll(tasks);
            for (Future<String> result : results) {
                require(result.isDone(), "not done");
                require(result.get().equals(CppCompiler.COMPILED), result.get());
            }
            executor.shutdown();
        } catch (InterruptedException | ExecutionException e) {
            require(false, e.getMessage());
        }

    }
}

class CppCompiler implements Callable<String> {

    public static final String COMPILED = "Compiled";
    public static final String GCC = "g++ -O2 -static -std=c++11";

    private final File file;
    private final String header;

    public CppCompiler(File file, String header) {
        this.file = file;
        this.header = header;
    }


    @Override
    public String call() {
        try {

            BookMaker.replaceTabs(file, BookMaker.TABS_WIDTH);

            String code = BookMaker.readAll(file);
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
            BookMaker.require(false, e.getMessage());
        }
        return COMPILED;
    }
}

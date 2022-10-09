import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {

    public static void main(String[] args) {
        // Pega instancia do banheiro
        Bathroom bathroom = Bathroom.getInstance();

        // Inicializa lista de pessoas
        List<Person> users = new ArrayList<>();

        // Cria pessoas para usar o banheiro
        for (int i = 0; i < ((new Random()).nextInt(10) + 15); i++) {
            // Cria homens e mulheres
            if (new Random().nextInt(2) == 0) {
                // Cria homem
                users.add(new Person(("João #" + i), Sex.MAN, bathroom));
            } else {
                // Cria mulher
                users.add(new Person(("Maria #" + i), Sex.WOMAN, bathroom));
            }
        }
        // Start threads de pessoas
        users.stream().map(Thread::new).forEach(Thread::start);
        users.stream().map(Thread::new).forEach((t) -> {
            try {
                t.join();
            } catch (InterruptedException ex) {
                Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
            }
        });
    }
}

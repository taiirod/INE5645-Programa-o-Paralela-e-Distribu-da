import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.LinkedHashSet;
import java.util.concurrent.Semaphore;
import java.util.logging.Level;
import java.util.logging.Logger;


public class Bathroom {

    // Capacidade do banheiro
    private static final int CAPACITY = 5;

    // Singleton
    private static final Bathroom instance = new Bathroom(CAPACITY);
    // Capacidade do banheiro
    private final int capacity;
    // Sexo atual do banheiro
    private Sex currentSex;
    // Lista de usuarios no banheiro
    private final LinkedHashSet<Person> users;

    // Semaphore
    private final Semaphore semaphore;
    private final Semaphore leftMutex;
    private final Semaphore enterMutex;

    public Bathroom(int capacity) {
        this.capacity = capacity;
        this.currentSex = Sex.NONE;
        this.users = new LinkedHashSet<>();
        this.semaphore = new Semaphore(this.capacity, true);
        this.leftMutex = new Semaphore(1, true);
        this.enterMutex = new Semaphore(1, true);
    }

    // Adiciona usuário no banheiro
    public void addUser(Person person) {
        try {
            this.semaphore.acquire();
        } catch (InterruptedException ex) {
            Logger.getLogger(Bathroom.class.getName())
                    .log(Level.SEVERE, null, ex);
        }
        
        // Verifica se banheiro está vazio e define a sexualidade atual do banheiro
        if (this.isEmpty()) {
            this.currentSex = person.getSex();
        }
        
        // Verifica se o banheiro nao está cheio e se a sexualidade da pessoa é a permitida no banheiro no momento
        if (!this.isFull() && !this.users.contains(person)
                && getCurrentSex().equals(person.getSex())) {
            try {
                this.enterMutex.acquire();
            } catch (InterruptedException ex) {
                Logger.getLogger(Bathroom.class.getName())
                        .log(Level.SEVERE, null, ex);
            }
            
            // Adiciona pessoa no banheiro
            if (this.users.add(person)) {
                System.out.println(person.getName() + " - Entrou no banheiro. | " + getTime());
            }
            this.enterMutex.release();
            
            // Verifica se o banheiro está cheio
            if (this.isFull()) {
                System.out.println(">>>>> O banheiro está cheio <<<<<");
            }
        }
    }

    // Remove usuário do banheiro
    public void removeUser(Person person) {
        this.semaphore.release();
        
        // Verifica se banheiro não está vazio
        if (!this.isEmpty()) {
            try {
                this.leftMutex.acquire();
            } catch (InterruptedException ex) {
                Logger.getLogger(Bathroom.class.getName())
                        .log(Level.SEVERE, null, ex);
            }
            if (this.users.remove(person)) {
                System.out.println(person.getName() + " - Saiu do banheiro");
            }
            this.leftMutex.release();

            checkIfEmpty();
        }
    }

    // Verifica se banheiro está vazio
    private void checkIfEmpty() {
        if (this.isEmpty()) {
            System.out.println("----- O banheiro está vazio -----");
            this.currentSex = Sex.NONE;
        }
    }

    // Verifica se a pessoa está no banheiro
    public boolean isInTheBathroom(Person person) {
        return this.users.contains(person);
    }

    // Verifica se o banhairo está cheio
    public boolean isFull() {
        return this.capacity == this.users.size();
    }

    // Verifica se o banheiro está vazio
    public boolean isEmpty() {
        return this.users.isEmpty();
    }

    // Retorna o sexo atual do banheiro
    public Sex getCurrentSex() {
        return this.currentSex;
    }

    // Pega instancia do banheiro
    public static Bathroom getInstance() {
        return instance;
    }

    // Pega horario atual
    private static String getTime() {
        return LocalDateTime.now().format(DateTimeFormatter.ISO_LOCAL_TIME);
    }

    @Override
    public String toString() {
        return "Bathroom{" + "currentSex = " + this.currentSex
                + ", capacity = " + this.capacity
                + ", numberOfUsers = " + this.users.size() + '}';
    }

}

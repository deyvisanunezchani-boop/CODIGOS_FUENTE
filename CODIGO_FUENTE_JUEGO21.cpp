#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class Carta {
private:
    string nombre;
    string palo;
    int puntos;

public:
    Carta();
    Carta(string, string, int);

    void mostrarCarta();
    int obtenerPuntos();
};

Carta::Carta() {
    nombre = "";
    palo = "";
    puntos = 0;
}

Carta::Carta(string _nombre, string _palo, int _puntos) {
    if (_puntos >= 0 && _puntos <= 13) {
        nombre = _nombre;
        palo = _palo;
        puntos = _puntos;
    } else {
        nombre = "";
        palo = "";
        puntos = 0;
    }
}

void Carta::mostrarCarta() {
    cout << nombre << " de " << palo;
}

int Carta::obtenerPuntos() {
    return puntos;
}

class Mazo {
private:
    Carta cartas[52];
    int cantidad;

public:
    Mazo();
    void crearMazo();
    void barajar();
    Carta repartirCarta();
    int obtenerCantidad();
    void mostrarMazo();
};

Mazo::Mazo() {
    cantidad = 0;
}

void Mazo::crearMazo() {
    string palos[4] = {
        "Corazones", "Diamantes", "Treboles", "Picas"
    };

    string nombres[13] = {
        "A", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "J", "Q", "K"
    };

    int valores[13] = {
        11, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 10, 10, 10
    };

    cantidad = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            cartas[cantidad] =
                Carta(nombres[j], palos[i], valores[j]);
            cantidad++;
        }
    }
}

void Mazo::barajar() {
    for (int i = cantidad - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        Carta temporal = cartas[i];
        cartas[i] = cartas[j];
        cartas[j] = temporal;
    }
}

void Mazo::mostrarMazo() {
    cout << "\n===== MAZO BARAJADO =====" << endl;

    for (int i = 0; i < cantidad; i++) {
        cout << i + 1 << ". ";
        cartas[i].mostrarCarta();
        cout << " - Puntos: "
             << cartas[i].obtenerPuntos() << endl;
    }
}

Carta Mazo::repartirCarta() {
    if (cantidad <= 0) {
        cout << "No quedan cartas en el mazo." << endl;
        return Carta();
    }

    cantidad--;
    return cartas[cantidad];
}

int Mazo::obtenerCantidad() {
    return cantidad;
}

class Persona {
private:
    string nombre;
    Carta mano[52];
    int cantidadMano;
    int puntos;

    void calcularPuntos();

public:
    Persona(string);
    void recibirCarta(Carta);
    void repartirInicial(Mazo&);
    void jugarTurno(Mazo&);
    void jugarTurnoComputador(Mazo&);
    void mostrarMano();
    int obtenerPuntos();
    void mostrarResultado(Persona&);
};

Persona::Persona(string _nombre) {
    nombre = _nombre;
    cantidadMano = 0;
    puntos = 0;
}

void Persona::recibirCarta(Carta carta) {
    if (cantidadMano < 52) {
        mano[cantidadMano] = carta;
        cantidadMano++;
    }
}

void Persona::repartirInicial(Mazo& mazo) {
    if (mazo.obtenerCantidad() > 0) {
        recibirCarta(mazo.repartirCarta());
    }

    if (mazo.obtenerCantidad() > 0) {
        recibirCarta(mazo.repartirCarta());
    }
}

void Persona::calcularPuntos() {
    puntos = 0;
    int ases = 0;

    for (int i = 0; i < cantidadMano; i++) {
        puntos += mano[i].obtenerPuntos();

        if (mano[i].obtenerPuntos() == 11) {
            ases++;
        }
    }

    while (puntos > 21 && ases > 0) {
        puntos -= 10;
        ases--;
    }
}

void Persona::mostrarMano() {
    cout << "\nCartas de " << nombre << ":" << endl;

    for (int i = 0; i < cantidadMano; i++) {
        mano[i].mostrarCarta();
        cout << endl;
    }

    calcularPuntos();
    cout << "Puntos: " << puntos << endl;
}

int Persona::obtenerPuntos() {
    calcularPuntos();
    return puntos;
}

void Persona::jugarTurno(Mazo& mazo) {
    string opcion;

    while (obtenerPuntos() < 21) {
        if (mazo.obtenerCantidad() == 0) {
            cout << "\nNo quedan cartas en el mazo." << endl;
            break;
        }

        cout << "\nDeseas pedir otra carta? (s/n): ";
        cin >> opcion;

        if (opcion == "s" || opcion == "S") {
            recibirCarta(mazo.repartirCarta());
            mostrarMano();
        }
        else if (opcion == "n" || opcion == "N") {
            break;
        }
        else {
            cout << "Opcion no valida. Ingresa solamente s o n."
                 << endl;
        }
    }
}

void Persona::jugarTurnoComputador(Mazo& mazo) {
    cout << "\nTurno del Computador" << endl;
    mostrarMano();

    while (obtenerPuntos() < 17) {
        if (mazo.obtenerCantidad() == 0) {
            cout << "\nNo quedan cartas en el mazo." << endl;
            break;
        }

        cout << "\nEl Computador pide otra carta." << endl;
        recibirCarta(mazo.repartirCarta());
        mostrarMano();
    }
}

void Persona::mostrarResultado(Persona& Computador) {
    int puntosJugador = obtenerPuntos();
    int puntosComputador = Computador.obtenerPuntos();

    cout << "\n===== RESULTADO FINAL =====" << endl;
    cout << "Puntos del jugador: " << puntosJugador << endl;
    cout << "Puntos del Computador: " << puntosComputador << endl;

    if (puntosJugador > 21) {
        cout << "Gana la mesa. El jugador se paso de 21." << endl;
    }
    else if (puntosComputador > 21) {
        cout << "Gana el jugador. El Computador se paso de 21." << endl;
    }
    else if (puntosJugador > puntosComputador) {
        cout << "Gana el jugador." << endl;
    }
    else {
        cout << "Gana la mesa." << endl;
    }
}

int main() {
    srand(time(0));

    Mazo mazo;
    mazo.crearMazo();
    mazo.barajar();
    mazo.mostrarMazo();

    Persona jugador("Jugador");
    Persona Computador("Computador");

    jugador.repartirInicial(mazo);
    Computador.repartirInicial(mazo);

    jugador.mostrarMano();
    jugador.jugarTurno(mazo);

    Computador.jugarTurnoComputador(mazo);

    jugador.mostrarResultado(Computador);

    return 0;
}
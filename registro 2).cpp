
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

using namespace std;

int main() {

    time_t tiempoActual = time(0);
    tm* fechaHora = localtime(&tiempoActual);

    stringstream nombreArchivo;

    nombreArchivo << "registro_"
                  << fechaHora->tm_mday << "-"
                  << fechaHora->tm_mon + 1 << "-"
                  << fechaHora->tm_year + 1900 << "_"
                  << fechaHora->tm_hour << "-"
                  << fechaHora->tm_min << "-"
                  << fechaHora->tm_sec
                  << ".txt";

    ifstream datos("datos.txt");

    if (!datos.is_open()) {
        cout << "Error: no se pudo abrir datos.txt" << endl;
        return 1;
    }

    string punto;
    float caudal[3];
    float nivel[3];

    bool leido[3] = {false, false, false};

    while (datos >> punto) {

        float q, n;
        datos >> q >> n;

        int posicion = -1;

        if (punto == "P1") {
            posicion = 0;
        }
        else if (punto == "P2") {
            posicion = 1;
        }
        else if (punto == "P3") {
            posicion = 2;
        }

        if (posicion != -1 && datos) {
            caudal[posicion] = q;
            nivel[posicion] = n;
            leido[posicion] = true;
        }
    }

    datos.close();

    if (!leido[0] || !leido[1] || !leido[2]) {
        cout << "Error: faltan mediciones." << endl;
        return 1;
    }

    cout << "===== SISTEMA DE ALCANTARILLADO =====" << endl;

    for (int i = 0; i < 3; i++) {

        cout << "P" << i + 1
             << " | Caudal: " << caudal[i]
             << " L/s | Nivel: " << nivel[i]
             << " cm" << endl;
    }

     float TOLERANCIA = 10.0;
     float AUMENTO_NIVEL = 20.0;
     float NIVEL_MAXIMO = 8.0;

    float variacionCaudal = 0;
    float variacionNivel = 0;

    if (caudal[0] > 0) {
        variacionCaudal =
            ((caudal[0] - caudal[2]) / caudal[0]) * 100;
    }

    if (nivel[0] > 0) {
        variacionNivel =
            ((nivel[2] - nivel[0]) / nivel[0]) * 100;
    }

    cout << "\nVariacion del caudal: "
         << variacionCaudal << "%" << endl;

    cout << "Variacion del nivel: "
         << variacionNivel << "%" << endl;

    string alerta;

    if (nivel[0] >= NIVEL_MAXIMO ||
        nivel[1] >= NIVEL_MAXIMO ||
        nivel[2] >= NIVEL_MAXIMO) {

        alerta = "SOBRECARGA: nivel de agua elevado.";

    }
    else if (variacionCaudal > TOLERANCIA &&
             variacionNivel > AUMENTO_NIVEL) {

        alerta = "POSIBLE OBSTRUCCION: disminucion del caudal "
                 "y aumento del nivel.";

    }
    else if (variacionCaudal > TOLERANCIA) {

        alerta = "POSIBLE FUGA: disminucion significativa "
                 "del caudal.";

    }
    else {

        alerta = "FUNCIONAMIENTO NORMAL: no se detectaron "
                 "anomalias.";

    }

    cout << "\n===== RESULTADO =====" << endl;
    cout << alerta << endl;

    ofstream registro(nombreArchivo.str());

    if (registro.is_open()) {

        registro << "====================================" << endl;
        registro << " SISTEMA DE MONITOREO DE ALCANTARILLADO"
                 << endl;

        registro << "Fecha: "
                 << fechaHora->tm_mday << "/"
                 << fechaHora->tm_mon + 1 << "/"
                 << fechaHora->tm_year + 1900 << endl;

        registro << "Hora: "
                 << fechaHora->tm_hour << ":"
                 << fechaHora->tm_min << ":"
                 << fechaHora->tm_sec << endl;

        registro << "====================================" << endl;

        registro << "\nMediciones:" << endl;

        for (int i = 0; i < 3; i++) {

            registro << "P" << i + 1
                     << " | Caudal: " << caudal[i]
                     << " L/s | Nivel: " << nivel[i]
                     << " cm" << endl;
        }

        registro << "\nVariacion del caudal: "
                 << variacionCaudal << "%" << endl;

        registro << "Variacion del nivel: "
                 << variacionNivel << "%" << endl;

        registro << "\nResultado: " << alerta << endl;

        registro << "====================================" << endl;

        registro.close();

        cout << "\nRegistro creado: "
             << nombreArchivo.str() << endl;

    }
    else {
        cout << "\nError al crear el archivo de registro." << endl;
    }

    return 0;
}

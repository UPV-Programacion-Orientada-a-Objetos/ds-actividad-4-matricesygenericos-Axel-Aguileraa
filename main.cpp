#include <iostream>
#include <iomanip> 
#include <cmath>   


template <typename T>
class Campo2D {
private:
    T **_datos;       // Puntero doble para la matriz dinámica
    int _filas;
    int _columnas;

    // Método privado para liberar toda la memoria de la matriz
    void liberarMemoria() {
        if (_datos != nullptr) {
            for (int i = 0; i < _filas; ++i) {
                delete[] _datos[i]; // Libera cada fila (columnas)
            }
            delete[] _datos; // Libera el arreglo de punteros 
            _datos = nullptr;
        }
    }

public:
    Campo2D(int f, int c) : _filas(f), _columnas(c) {
        _datos = new T*[_filas];
        
        for (int i = 0; i < _filas; ++i) {
            _datos[i] = new T[_columnas](); 
        }
    }

    // Destructor: Libera toda la memoria asignada
    ~Campo2D() {
        liberarMemoria();
    }

    void setValor(int f, int c, T valor) {
        if (f >= 0 && f < _filas && c >= 0 && c < _columnas) {
            _datos[f][c] = valor;
        }
    }

    // Obtiene el valor de una celda, validando índices
    T getValor(int f, int c) const {
        if (f >= 0 && f < _filas && c >= 0 && c < _columnas) {
            return _datos[f][c];
        }
        return T(); 
    }

    void imprimir() const {
        for (int i = 0; i < _filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < _columnas; ++j) {
                std::cout << std::fixed << std::setprecision(1) << _datos[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }

    void redimensionar(int nuevaF, int nuevaC) {
        T** nuevaMatriz = new T*[nuevaF];
        for (int i = 0; i < nuevaF; ++i) {
            nuevaMatriz[i] = new T[nuevaC](); // Inicializa a cero
        }

        int filasACopiar = (_filas < nuevaF) ? _filas : nuevaF;
        int columnasACopiar = (_columnas < nuevaC) ? _columnas : nuevaC;

        for (int i = 0; i < filasACopiar; ++i) {
            for (int j = 0; j < columnasACopiar; ++j) {
                nuevaMatriz[i][j] = _datos[i][j];
            }
        }
        
        liberarMemoria();

        _datos = nuevaMatriz;
        _filas = nuevaF;
        _columnas = nuevaC;
    }

    T calcularGradientePromedio(int inicioF, int finF, int inicioC, int finC) {
        if (inicioF < 0 || finF >= _filas || inicioC < 0 || finC >= _columnas || inicioF > finF || inicioC > finC) {
            std::cerr << "Error: Región de gradiente inválida." << std::endl;
            return T();
        }

        T sumaTotalGradientes = 0;
        int puntosCalculados = 0;

        for (int i = inicioF; i <= finF; ++i) {
            for (int j = inicioC; j <= finC; ++j) {
                T sumaDiferenciasVecinos = 0;
                int vecinosConsiderados = 0;

                if (j + 1 <= finC) { 
                    sumaDiferenciasVecinos += std::abs(_datos[i][j+1] - _datos[i][j]);
                    vecinosConsiderados++;
                }

                if (i + 1 <= finF) { 
                    sumaDiferenciasVecinos += std::abs(_datos[i+1][j] - _datos[i][j]);
                    vecinosConsiderados++;
                }
                
                if (vecinosConsiderados > 0) {
                    sumaTotalGradientes += (sumaDiferenciasVecinos / vecinosConsiderados);
                    puntosCalculados++;
                }
            }
        }

        if (puntosCalculados > 0) {
            return sumaTotalGradientes / puntosCalculados;
        }
        
        return T(); 
    }
};

int main() {
    std::cout << "--- Simulador Genérico de Campo 2D ---" << std::endl;
    
    // PRUEBA CON TIPO FLOAT
    std::cout << "\n>> Inicializando Campo Gravitatorio (Tipo FLOAT) <<" << std::endl;
    
    Campo2D<float> campoFloat(3, 3);
    
    std::cout << "Creando Grid (float) de 3x3..." << std::endl;
    std::cout << "Estableciendo valores iniciales..." << std::endl;
    campoFloat.setValor(0, 0, 10.0f); campoFloat.setValor(0, 1, 8.0f); campoFloat.setValor(0, 2, 5.0f);
    campoFloat.setValor(1, 0, 12.0f); campoFloat.setValor(1, 1, 9.0f); campoFloat.setValor(1, 2, 6.0f);
    campoFloat.setValor(2, 0, 15.0f); campoFloat.setValor(2, 1, 11.0f); campoFloat.setValor(2, 2, 7.0f);
    
    std::cout << "\nGrid Actual (Paso 0):" << std::endl;
    campoFloat.imprimir();

    std::cout << "\nOpción: Calcular Gradiente Promedio" << std::endl;
    std::cout << "Ingrese Fila Inicial: 0, Fila Final: 2" << std::endl;
    std::cout << "Ingrese Columna Inicial: 0, Columna Final: 2" << std::endl;
    std::cout << "\nCalculando Gradiente Promedio en la región [0,2]x[0,2]..." << std::endl;
    float gradiente = campoFloat.calcularGradientePromedio(0, 2, 0, 2);
    std::cout << "Gradiente Promedio calculado: " << std::fixed << std::setprecision(5) << gradiente << " unidades/metro." << std::endl;

    std::cout << "\nOpción: Redimensionar (A una dimensión mayor)" << std::endl;
    std::cout << "Redimensionando Grid a 4x4..." << std::endl;
    campoFloat.redimensionar(4, 4);
    std::cout << "Datos copiados. Memoria antigua liberada." << std::endl;
    campoFloat.imprimir();

    std::cout << "\nOpción: Redimensionar (A una dimensión menor)" << std::endl;
    std::cout << "Redimensionando Grid a 2x2..." << std::endl;
    campoFloat.redimensionar(2, 2);
    std::cout << "Datos copiados. Memoria antigua liberada." << std::endl;
    campoFloat.imprimir();

    // PRUEBA CON TIPO INT
    std::cout << "\n\n>> Inicializando Campo de Potencial (Tipo INT) <<" << std::endl;
    Campo2D<int> campoInt(2, 4);
    std::cout << "Creando Grid (int) de 2x4..." << std::endl;

    std::cout << "\nGrid Actual (Paso 0):" << std::endl;
    campoInt.imprimir();

    std::cout << "\nOpción: Salir" << std::endl;
    // Los destructores se llaman automáticamente aquí
    std::cout << "Sistema cerrado." << std::endl;

    return 0;
}

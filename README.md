# 🏗️ Algoritmo del Abusón en C

## 📌 Estructura del Programa

El programa realizado es `algoritmoAbuson.c`. Está organizado en las siguientes secciones:

- 📌 **Declaración de constantes y estructuras** para los nodos.
- 🔄 **Inicialización de los nodos**, asignando un identificador único y un estado aleatorio (vivo o caído).
- 📡 **Lógica del Algoritmo del Abusón**, basada en la comunicación entre nodos.
- 🧵 **Sincronización con hilos (`pthread`)**, para emular el comportamiento distribuido de los nodos.
- 🏁 **Impresión del resultado final**, mostrando qué nodos están activos y cuál es el líder elegido.

---

## 🛠️ Funciones Principales

### 🔹 `inicializar_nodos()`
📌 Crea un arreglo de nodos y asigna a cada uno un identificador único (1 al total de nodos).  
🎲 Determina aleatoriamente si están vivos o caídos.  
❌ Los nodos caídos no responden a mensajes, simulando fallos en el sistema.  
🎲 Usa `rand()` para la aleatoriedad.

### 🔹 `enviar_mensaje()`
📡 Simula la comunicación entre nodos:  
✅ Si el destinatario está **vivo**, responde al emisor.  
❌ Si el destinatario está **caído**, no responde, y el emisor lo marca como **inactivo** tras un tiempo límite.  
⏳ Este tiempo límite se implementa con retrasos simulados.

### 🔹 `algoritmo_abuson()`
🤖 Función principal del algoritmo.  
🔎 Cada nodo revisa si hay otros nodos con identificadores **mayores y vivos**.  
👑 Si no encuentra ninguno, **se autoproclama líder** y notifica a los demás nodos.  
✅ Garantiza que el nodo activo con el identificador **más alto** sea el líder.

### 🔹 `imprimir_estado_final()`
📜 Muestra el estado final del sistema:  
- ✅ Lista de nodos vivos.  
- ❌ Lista de nodos caídos.  
- 👑 **Nodo elegido como líder**.

---

## 🏗️ `main`: Algoritmo del Abusón

La función `main()` es el **punto de entrada** del programa y sigue estos pasos:

1️⃣ **Inicializa los nodos** llamando a `inicializar_nodos()`.  
2️⃣ **Crea un hilo** para cada nodo con `pthread_create()`, ejecutando el algoritmo en paralelo.  
3️⃣ **Espera a que los hilos terminen** con `pthread_join()`.  
4️⃣ **Imprime el resultado final** llamando a `imprimir_estado_final()`.  

---

## 🔒 Sincronización

⚠️ Para evitar **inconsistencias**, se usa **`pthread_mutex`** al acceder a variables compartidas.  
🛡️ Esto **garantiza operaciones atómicas y seguras**, protegiendo el estado de los nodos y el identificador del líder.

---

## 🏃‍♂️ Ejecución del Programa

### 📦 Instalación de Bibliotecas

📌 El programa utiliza la biblioteca estándar de **hilos de POSIX (`pthread`)**.  
✅ No requiere dependencias adicionales.

### 🖥️ Instrucciones para Compilar y Ejecutar

Para ejecutar el programa, sigue estos pasos:

1️⃣ Abre el archivo `algoritmoAbuson.c`.  
2️⃣ Compila el programa con el siguiente comando:

   ```sh
   gcc algoritmoAbuson.c -o algoritmoAbuson -lpthread
  ```

Y para ejecutar usamos:
```sh
./algoritmoAbuson
  ```


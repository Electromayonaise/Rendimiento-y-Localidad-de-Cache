# Impacto de la Localidad de Caché en el Rendimiento de Suma de Matrices

## Curso: Arquitectura de Computadores

## **Autores** 
- [Alejandro Mejía Mejía](https://github.com/SrCracles)
- [Martín Gómez Caicedo](https://github.com/Electromayonaise)
- [Juan Camilo Muñoz](https://github.com/JuanCamiloMunozB)
- [Manuel Cardona](https://github.com/JManuel2004)

## Descripción del Proyecto

Este proyecto se centra en el análisis del impacto de la localidad de caché en el rendimiento de diferentes algoritmos de multiplicación de matrices. Se diseña y realiza un experimento para medir el desempeño de seis versiones de algoritmos de multiplicación de matrices, cada uno con distinto nivel de optimización en el uso de caché. El objetivo es evaluar cómo la estructura de memoria y la jerarquía de caché influyen en el tiempo de ejecución y cómo diferentes configuraciones pueden mejorar el rendimiento en sistemas de cómputo.

## Objetivos

1. **Evaluar el impacto de la localidad de caché** en el tiempo de ejecución de seis algoritmos de multiplicación de matrices.
2. **Diseñar y realizar un experimento controlado** para medir el rendimiento en diferentes configuraciones de hardware y parámetros de experimentación.
3. **Analizar los datos y proponer mejoras** en la implementación de algoritmos, aplicando técnicas como la multiplicación por bloques para optimizar el rendimiento del algoritmo menos eficiente.

## Metodología

Para el experimento, se sigue una metodología estructurada basada en diseño experimental:

1. **Identificación del problema**: Evaluar cómo la estructura de caché afecta el rendimiento en la multiplicación de matrices.
2. **Selección de factores y niveles**:
   - **Versión del algoritmo**: Seis versiones (a, b, c, d, e, y f).
   - **Tamaño de la matriz de datos**.
   - **Tipo de dato**: Float y Double.
   - **Tipo de ISA**: Arquitecturas x86 y x64.
3. **Experimentación preliminar**: Validación de las configuraciones experimentales.
4. **Realización del experimento**: Ejecución de las pruebas en sistemas de cómputo personales, manteniendo los datos de cada unidad experimental de forma independiente para realizar un análisis comparativo.
5. **Análisis de datos**: Análisis estadístico y comparación entre unidades experimentales.
6. **Propuesta de mejora**: Implementación de multiplicación por bloques en la versión menos eficiente y evaluación de su impacto en el rendimiento.

## Prácticas y Laboratorios Relacionados

Este proyecto se apoya en prácticas y laboratorios realizados durante el curso, que incluyen:

- **Semana 2**: Introducción a ANOVA y regresión.
- **Semana 3**: Replanteamiento de experimento (aleatorización, factores, niveles).
- **Semana 4**: Conducción de experimentos.
- **Semanas 13-16**: Análisis de datos en Minitab, pruebas de overclocking, y configuración experimental avanzada.

## Requisitos

- **Hardware**: Equipos personales con arquitecturas x86 y x64.
- **Software**: Python, C++, herramientas de análisis estadístico (Minitab, Excel).

## Estructura de Archivos

- `/src`: Contiene los scripts en Python y archivos C++ para la ejecución de los algoritmos de multiplicación de matrices.
- `/data`: Archivos .xls con los datos obtenidos en los experimentos.
- `/analysis`: Informe de análisis de resultados y proyecto de Minitab.
- `/docs`: Documentación y recursos adicionales.

## Referencias

1. Randal E. Bryant, David R. O'Hallaron. *Computer Systems: A Programmer's Perspective*, 2010.
2. Douglas C. Montgomery. *Diseño y análisis de experimentos*.
3. John Riley. *Writing Fast Programs: A Practical Guide for Scientists and Engineers*.

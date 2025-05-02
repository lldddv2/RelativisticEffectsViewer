# Usamos Ubuntu como base
FROM ubuntu:22.04

# Instalar dependencias
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    build-essential \
    wget \
    curl \
    python3 \
    make

# Instalar cglm
WORKDIR /tmp
RUN git clone https://github.com/recp/cglm.git && \
    cd cglm && \
    cmake . && \
    make && \
    make install

# Crear directorio para emsdk pero no en /app (que será montado)
RUN mkdir -p /opt/emsdk

# Clonar e instalar emsdk
WORKDIR /opt/emsdk
RUN git clone https://github.com/emscripten-core/emsdk.git .
RUN ./emsdk install 3.1.45 && \
    ./emsdk activate 3.1.45

# Aquí está el truco: pre-cargar el entorno en el .bashrc
RUN echo 'source /opt/emsdk/emsdk_env.sh' >> /root/.bashrc

# Variables de entorno permanentes
ENV PATH="/opt/emsdk:/opt/emsdk/upstream/emscripten:/opt/emsdk/node/14.18.2_64bit/bin:${PATH}"
ENV EMSDK="/opt/emsdk"
ENV EM_CONFIG="/opt/emsdk/.emscripten"
ENV EMSDK_NODE="/opt/emsdk/node/14.18.2_64bit/bin/node"

# Directorio de trabajo para tu código
WORKDIR /app

# No es necesario source, el entorno ya está configurado
CMD ["make"]
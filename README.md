# jogo.h - Biblioteca de alto nível para desenvolvimento de jogos em C

<p align="left">
    <a href="https://syndelis.github.io/jogo.h" alt="Documentação">
    <img src="https://img.shields.io/static/v1?label=Docs&message=GitHub Pages&color=blue&style=for-the-badge&logo=gitbook" />
    </a>
</p>

- [Sobre a biblioteca](#sobre-a-biblioteca-)
- [Como usar](#como-usar-)
- [Instalando as Dependências](#instalando-as-dependências-)
    - [Debian/Ubuntu/Pop_OS!/ElementaryOS](#distribuições-baseadas-em-debian-ubuntupop_oselementaryos)
    - [ArchLinux](#distribuições-baseadas-em-archlinux)
    - [Window](#windows)
- [Documentação](#documentação-)

---

## Sobre a biblioteca [▲]

A biblioteca **`jogo.h`** oferece uma API de alto nível para o gerenciamento de
janelas, renderização de figuras geométricas e texturas (imagens e GIFs), 
reprodutor de áudio (todos os formatos mais comuns, como MP3, OGG e WAV) e
sistema de interação com o usuário.

O objetivo da biblioteca é abstrair todas as tecnologias de baixo nível,
apresentando ao programador uma interface que é direta ao ponto. Exemplo de
funções da sua API são:

- `abre_janela(largura, altura);`
- `desenha_imagem(x, y, "personagem.gif");`
- `toca_som_em_loop("musica.mp3");`
- `tecla_acabou_de_ser_apertada("A");`
- `printf("Coordenadas do mouse: %d, %d\n", mouse_x, mouse_y);`

---

## Como usar [▲]

Este projeto possui pouquíssimas
[dependências externas](#dependências-e-compilação) e compila para uma 
biblioteca estática **`libjogo.a`**. Para compilar qualquer programa que use a
bibliteca, basta incluí-la nas fontes do GCC/Clang e adicionar os includes e
links gerados pelo makefile.

Exemplo de programa e compilação:

```c
// main.c
#include <jogo.h>

int main() {
    abre_janela(640, 320);

    while (janela_esta_aberta())
        if (tecla_pressionada(ESC))
            janela_deve_fechar();

    fecha_janela();
}
```

O programa acima simplesmente abre uma janela e a fecha caso o usuário tecle
**Esc**. Para compilá-lo, iremos supor que a biblioteca **`jogo.h`** está no
diretório **`lib/jogo/`**

```sh
$ make -C lib/jogo

$ gcc main.c lib/jogo/libjogo.a $(make --quiet -C lib/jogo includes) $(make --quiet -C lib/jogo links)
```

Como demonstrado, para que você não precise pesquisar quais são os caminhos de 
*include* e *link*, o **`Makefile`** da biblioteca provê as seguintes regras:

- **`includes`**: Retorna todos os caminhos de inclusão para o compilador;
- **`links`**: Retorna todos os links para o linker;

---

## Instalando as Dependências [▲]

A biblioteca possui dependência dos seguintes pacotes:
- Algum compilador de C (gcc, clang);
- Comando utilitário `make`;
- Comando utilitário `cmake`;
- *GL Extension Wrangler* **GLEW**;
- Pacotes de desenvolvimento equivalentes do seu servidor gráfico (X11 ou Wayland);

Portanto, para instalar as dependências, basta rodas os comandos abaixo. Note que o **`$`** antes de cada linha simboliza apenas um comando, e não deve ser copiado.

### Distribuições baseadas em Debian (Ubuntu/Pop_OS!/ElementaryOS):
```sh
$ sudo apt install build-essential cmake libglew-dev
$ case $XDG_SESSION_TYPE in
    x11) sudo apt install xorg-dev ;;
    wayland) sudo apt install libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules;;
    *) echo "Não foi possível determinar o tipo de sessão que está sendo usada" ;;
esac
```

### Distribuições baseadas em ArchLinux:
```sh
$ sudo pacman -S glew
$ case $XDG_SESSION_TYPE in
    x11) sudo pacman -S libx11 ;;
    wayland) sudo pacman -S wayland ;;
    *) echo "Não foi possível determinar o tipo de sessão que está sendo usada" ;;
esac
```

### Windows

Compilar diretamente no Windows ainda está em fase de testes. No momento, recomenda-se que o programador instale o [WSL 2](https://aka.ms/wsl) e compile e execute o programa pelo terminal de Linux da distribuição escolhida.

## Documentação [▲]

A documentação está disponível neste link: **[docs]**.

Ela foi feita usando [Doxygen](https://doxygen.nl) com o tema
[Doxygen Awesome CSS](https://jothepro.github.io/doxygen-awesome-css/index.html). Para *buildar* a documentação, instale `doxygen` e execute

```sh
$ make docs
```

A documentação estará disponível em HTML e como páginas do
[manual](https://www.kernel.org/doc/man-pages/) Linux.

[▲]: #jogoh---biblioteca-de-alto-nível-para-desenvolvimento-de-jogos-em-c
[docs]: https://syndelis.github.com/jogo.h
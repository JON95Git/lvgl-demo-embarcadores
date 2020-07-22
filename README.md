# LVGL Demo embarcadores
Aplicação de exemplo usando a biblioteca gráfica LVGL.

# Introdução
O presente projeto foi desenvolvido com o intuito de demonstrar a utlização da biblioteca gráfica open source [LVGL](https://github.com/lvgl/lvgl). O código é portável entre plataformas `x86`(simulador) e `arm` (STM32), mediante configuração de build.

O projeto é compatível com a ferramenta de geração de código [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html).


# Dependências
## Hardware
Para executar o projeto será necessário:
- Placa [STM32F429-Discovery](https://www.st.com/en/evaluation-tools/32f429idiscovery.html)

## Software
Para realizar a compilação e execução do projeto será necessário:
- Um computador `host` rodando algum sistema operacional `GNU/Linux`
- Compilador `arm-none-gnueabi-gcc` para microcontroladores `arm cortex M`
- Ferramentea `make` para utilização de arquivos `Makefile`
- Biblioteca [LVGL](https://github.com/lvgl/lvgl) instalado no diretório `app/gui`
- Ferramenta `openOCD` para gravação e depuração do firmware

## Simulador
Caso o usuário não disponha do hardware necessário, pode se utilizar o [simulador](https://github.com/lvgl/lv_sim_eclipse_sdl) para rodar apenas a aplicação gráfica de alto nível diretamente no computador host. A aplicação gráfica está isolada em `app/gui`.

Para utilizar o `simulador` é necessário fazer com que os arquivos presentes em `app/gui` estejam visíveis para o `simulador`. Isso pode ser feito utilizando `links simbólicos`.

Para criar `links simbólicos` da aplicação gráfica para o `simulador`, execute:

```
ln -s ."path-to-embedded-project/gui/*.c" "path-to-simulator-project/gui/"
ln -s ."path-to-embedded-project/gui/*.h" "path-to-simulator-project/gui/"
ln -s ."path-to-embedded-project/gui/components/*.c" "path-to-simulator-project/gui/components/"
ln -s ."path-to-embedded-project/gui/components/*.h" "path-to-simulator-project/gui/components/"
```

Dessa forma, toda alteração realizada em qualquer um dos projetos (`firmware` ou `simulador`) será vista por ambas as partes.
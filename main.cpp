#include <iostream>
#include <SDL3/SDL.h>
#include <Windows.h>

struct BubbleSortState
{
    bool sorted;
    int currentOuter;
    int currentInner;
    int swappedLeft;
    int swappedRight;
};

bool StepBubbleSort(BubbleSortState* state, int* list, int listSize)
{
    state->swappedLeft = -1;
    state->swappedRight = -1;
     if (!state->sorted)
        {
            
            if (state->currentInner < listSize - state->currentOuter - 1)
            {
                if (list[state->currentInner] > list[state->currentInner+1])
                {
                    std::swap(list[state->currentInner], list[state->currentInner+1]);
                    state->swappedLeft = state->currentInner;
                    state->swappedRight = state->currentInner + 1;
                }
                state->currentInner++;
            }
            else 
            {
                state->currentInner = 0;
                state->currentOuter++;
                if (state->currentOuter >= listSize-1)
                {
                    state->sorted = true;
                }
                state->swappedLeft = -1;
                state->swappedRight = -1;
            }
        }
    return state->sorted ? false : true;    
}

void RenderSort(SDL_Renderer* renderer, int swappedLeft, int swappedRight, int* list, int listSize, int maxValue, int windowWidth, int windowHeight, float spacing)
{
        SDL_FRect rect = {};
        rect.w = ((float) windowWidth - ((float)(listSize-1)*spacing)) / (float) listSize;

        for (int i = 0; i < listSize; i++)
        {
            if (i == swappedLeft || i == swappedRight) 
            {
                SDL_SetRenderDrawColor(renderer, 255, 0,0,0);
            }
            else 
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            }
            int value = list[i];
            rect.x = i * (rect.w + spacing);
            rect.h = ( (float) value / (float)(maxValue)) * (float)(windowHeight);
            rect.y = windowHeight - rect.h;
            SDL_RenderFillRect(renderer, &rect);
        }
}

void RenderSortCompleted(SDL_Renderer* renderer, int* list, int listSize, int maxValue, int windowWidth, int windowHeight, float spacing, int* step)
{
        SDL_FRect rect = {};
        rect.w = ((float) windowWidth - ((float)(listSize-1)*spacing)) / (float) listSize;
        for (int i = 0; i < listSize; i++)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            if (i <= *step)
            {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
            }
            int value = list[i];
            rect.x = i * (rect.w + spacing);
            rect.h = ( (float) value / (float)(maxValue)) * (float)(windowHeight);
            rect.y = windowHeight - rect.h;
            SDL_RenderFillRect(renderer, &rect);
        }
        *step += 1;
}

float StringToFloat32(char* str)
{
    double value = 0.0;
    bool inFraction = false;
    int decimalCount = 0;
    for (int i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == '.')
        {
            inFraction = true;
            continue;
        }
        int digit = str[i] - '0';
        value = value * 10 + digit;
        if (inFraction)
        {
            decimalCount++; 
        }
    }
    value = value / std::pow(10, decimalCount);
    return (float) value;
}

unsigned int StringToUInt32(char* str)
{
    int value = 0;
    for (int i = 0; str[i] != '\0'; ++i)
    {
        value = value * 10 + (str[i] - '0');
    }
    return value;
}

int main(int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Could not initialize SDL: %s" , SDL_GetError());
        return -1;
    }
    SDL_Window *window = SDL_CreateWindow("Test Window", 1920, 1080, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("Could not create window: %s", SDL_GetError());
        return -1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        return -1;
    }

    const int number_count = StringToUInt32(argv[1]);
    int maxValue = 0;
    int* numbers = (int*) std::malloc(number_count*sizeof(int));
        for (int i = 0; i < number_count; i++)
        {   
            numbers[i] = SDL_rand(1000);
            if (numbers[i] > maxValue)
            {
                maxValue = numbers[i];
            }
        }
    BubbleSortState sortState = {};
    sortState.swappedLeft = -1;
    sortState.swappedRight = -1;        
    bool running = true;
    int sortCompletedStepValue = 0;
    int* sortCompletedStep = &sortCompletedStepValue;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }
        int windowWidth;
        int windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        // Set black background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        if (!sortState.sorted)
        {
            StepBubbleSort(&sortState, numbers, number_count);
            RenderSort(renderer, sortState.swappedLeft, sortState.swappedRight, numbers, number_count, maxValue, windowWidth, windowHeight, 2.0f);
            SDL_Delay(1);
        }
        else
        {
            RenderSortCompleted(renderer, numbers, number_count, maxValue, windowWidth, windowHeight, 2.0f, sortCompletedStep);
            SDL_Delay(5);
        }
        SDL_RenderPresent(renderer);
    }
    std::free(numbers);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
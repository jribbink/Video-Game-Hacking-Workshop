#include <Windows.h>
#include <stdio.h>

struct glow_t {
    int m_nNextFreeSlot;
    int pEntity;
    float r;
    float g;
    float b;
    float a;
    bool m_bGlowAlphaCappedByRenderAlpha;
    float m_flGlowAlphaFunctionOfMaxVelocity;
    float m_flGlowAlphaMax;
    float m_flGlowPulseOverdrive;
    bool m_bRenderWhenOccluded;
    bool m_bRenderWhenUnoccluded;
    bool m_bFullBloomRender;
    int m_nFullBloomStencilTestValue;
    int m_nRenderStyle;
    int m_nSplitScreenSlot;
};

int main(int argc, char** argv) {
    printf("%d", sizeof(glow_t));
    Sleep(2);
}
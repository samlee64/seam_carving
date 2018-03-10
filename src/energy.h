


FloatImage dualGradientEnergy(const FloatImage &im, bool clamp=true);
float dualGradientEnergy(const FloatImage &im, int x, int y, bool clamp=true);
FloatImage gradientEnergy(const FloatImage &im, bool clamp=true);


FloatImage createMaskedEnergyMap(FloatImage im, FloatImage mask, float value, bool isHorizontal);
FloatImage energyMap(FloatImage im, bool isHorizontal);
FloatImage createBlockedEnergyMap(FloatImage im, FloatImage block, int value, bool isHorizontal);

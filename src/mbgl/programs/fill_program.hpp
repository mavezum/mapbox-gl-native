#pragma once

#include <mbgl/programs/program.hpp>
#include <mbgl/programs/attributes.hpp>
#include <mbgl/programs/uniforms.hpp>
#include <mbgl/programs/textures.hpp>
#include <mbgl/util/geometry.hpp>
#include <mbgl/util/mat4.hpp>
#include <mbgl/util/size.hpp>
#include <mbgl/style/layers/fill_layer_properties.hpp>

#include <string>

namespace mbgl {

class ImagePosition;
class UnwrappedTileID;
class TransformState;
template <class> class Faded;

using FillLayoutAttributes = PositionOnlyLayoutAttributes;

using FillUniforms = TypeList<
    uniforms::u_matrix,
    uniforms::u_world>;

using FillPatternUniforms = TypeList<
    uniforms::u_matrix,
    uniforms::u_world,
    uniforms::u_texsize,
    uniforms::u_scale,
    uniforms::u_fade,
    uniforms::u_pixel_coord_upper,
    uniforms::u_pixel_coord_lower>;

class FillProgram : public Program<
    FillProgram,
    gfx::PrimitiveType::Triangle,
    FillLayoutAttributes,
    FillUniforms,
    TypeList<>,
    style::FillPaintProperties>
{
public:
    using Program::Program;

    static LayoutVertex layoutVertex(Point<int16_t> p) {
        return LayoutVertex {
            {{
                p.x,
                p.y
            }}
        };
    }
};

class FillPatternProgram : public Program<
    FillPatternProgram,
    gfx::PrimitiveType::Triangle,
    FillLayoutAttributes,
    FillPatternUniforms,
    TypeList<
        textures::u_image>,
    style::FillPaintProperties>
{
public:
    using Program::Program;

    static LayoutUniformValues layoutUniformValues(mat4 matrix,
                                                   Size framebufferSize,
                                                   Size atlasSize,
                                                   const CrossfadeParameters& crossfade,
                                                   const UnwrappedTileID&,
                                                   const TransformState&,
                                                   const float pixelRatio);
};

class FillOutlineProgram : public Program<
    FillOutlineProgram,
    gfx::PrimitiveType::Line,
    FillLayoutAttributes,
    FillUniforms,
    TypeList<>,
    style::FillPaintProperties>
{
public:
    using Program::Program;
};

class FillOutlinePatternProgram : public Program<
    FillOutlinePatternProgram,
    gfx::PrimitiveType::Line,
    FillLayoutAttributes,
    FillPatternUniforms,
    TypeList<
        textures::u_image>,
    style::FillPaintProperties>
{
public:
    using Program::Program;
};

using FillLayoutVertex = FillProgram::LayoutVertex;
using FillAttributes = FillProgram::AttributeList;

class FillLayerPrograms final : public LayerTypePrograms {
public:
    FillLayerPrograms(gfx::Context& context, const ProgramParameters& programParameters)
        : fill(context, programParameters),
          fillPattern(context, programParameters),
          fillOutline(context, programParameters),
          fillOutlinePattern(context, programParameters) {}
    ProgramMap<FillProgram> fill;
    ProgramMap<FillPatternProgram> fillPattern;
    ProgramMap<FillOutlineProgram> fillOutline;
    ProgramMap<FillOutlinePatternProgram> fillOutlinePattern;
};

} // namespace mbgl

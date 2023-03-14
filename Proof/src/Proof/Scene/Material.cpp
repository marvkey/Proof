#include "Proofprch.h"
#include "Material.h"

namespace Proof {

    

    Count<MaterialTable> MaterialTable::Copy()
    {
        Count<MaterialTable> table = Count<MaterialTable>::Create();
        for (auto [index,material] : m_Materials)
        {
            table->SetMaterial(index, material);
        }
        return table;
    }
    Material::Material()
    {
    }

    PhysicsMaterial::PhysicsMaterial()
    {
    }

    bool operator==(const MaterialTable& other, const MaterialTable& other1)
    {
        return other.GetMaterials().size() == other1.GetMaterials().size()
            && std::equal(other.GetMaterials().begin(), other.GetMaterials().end(),
                other1.GetMaterials().begin());
    }

}
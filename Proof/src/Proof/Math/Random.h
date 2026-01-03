#pragma once
#include <random>
#include <numeric>
#include <glm/glm.hpp>
static	std::random_device s_RandomDevice;
static	std::mt19937_64 s_RandomEngine64(s_RandomDevice());
namespace Proof
{

	struct Random 
	{
		inline static bool Bool() 
		{
			std::uniform_int_distribution<int> dist(0,1);
			return dist(s_RandomEngine64);
		}
		template<typename T>
		inline static T Real(T min = (std::numeric_limits<T>::min)(), T max = (std::numeric_limits<T>::max)()) 
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(s_RandomEngine64);
		};
		template<typename T>
		inline static T Int(T minNumber = (std::numeric_limits<T>::min)(),T maxNumber = (std::numeric_limits<T>::max)()) 
		{
			std::uniform_int_distribution<T> dist(minNumber, maxNumber);
			return dist(s_RandomEngine64);
		}

	    //https://github.com/NVIDIA-Omniverse/PhysX/blob/5ca9f472105a90d70d957c243cb0ef36fe251a9f/physx/snippets/snippetpathtracing/SnippetPathTracing.cpp#L452
		// Generates a random point inside a unit sphere (radius = 1).
		// This uses rejection sampling — random points are generated in a cube [-1,1],
		// and any that fall outside the sphere are discarded until a valid one is found.
		// Equivalent to Unity's Random.insideUnitSphere and PhysX's RandomInUnitSphere.
		inline static glm::vec3 InsideUnitSphere()
		{
			// Random float generator from -1.0 to 1.0 for each axis
			std::uniform_real_distribution<float> U(-1.0f, 1.0f);

			glm::vec3 p;

			do
			{
				// Generate a random point in a cube [-1, 1]^3
				p = { U(s_RandomEngine64), U(s_RandomEngine64), U(s_RandomEngine64) };

				// glm::dot(p, p) gives x² + y² + z² = distance squared from origin
				// If it's >= 1, the point lies outside the sphere → reject and retry
			}
			while (glm::dot(p, p) >= 1.0f); // p dot p is equal to magnituede^2 so if its magnitude is greater or equalt to 1

			// Return a random point inside the unit sphere
			return p;
		}

	    inline static glm::vec3 InsideSphere(float radius)
		{
		    return InsideUnitSphere() * radius;
		}

	    // https://github.com/NVIDIA-Omniverse/PhysX/blob/5ca9f472105a90d70d957c243cb0ef36fe251a9f/physx/snippets/snippetpathtracing/SnippetPathTracing.cpp#L461
	    inline static glm::vec3 UnitVector()
		{
		    // Generates a random direction uniformly distributed on the surface of the unit sphere.
		    // Uses the same logic as PhysX's RandomUnitVector 

		    std::uniform_real_distribution<float> U(-1.0f, 1.0f);
		    std::uniform_real_distribution<float> Angle(0.0f, 3.14159265358979323846264338327950288 *2.0f); // 2ppi

		    const float z = U(s_RandomEngine64);              // Random height between -1 and 1
		    const float a = Angle(s_RandomEngine64);          // Random angle around the Z axis
		    const float r = sqrtf(1.0f - z * z);              // Radius at this z (circle cross-section)

		    const float x = r * cosf(a);
		    const float y = r * sinf(a);

		    return glm::vec3(x, y, z);
		}
	};


    // basedon on LCG
    class RandomStream
    {
    public:
        // Default constructor
        RandomStream()
            : m_InitialSeed(0), m_Seed(0)
        {
        }

        // Constructor with an int32_t seed (can be negative)
        RandomStream(int32_t InSeed)
        {
            Initialize(InSeed);
        }

        // Initialize the generator with a seed
        void Initialize(int32_t InSeed)
        {
            m_InitialSeed = InSeed;
            m_Seed = static_cast<uint32_t>(InSeed);
        }

        // Reset to the initial seed
        void Reset()
        {
            m_Seed = static_cast<uint32_t>(m_InitialSeed);
        }

        // Get the initial seed
        int32_t GetInitialSeed() const
        {
            return m_InitialSeed;
        }

        // Generate a new random seed
        void GenerateNewSeed()
        {
            Initialize(static_cast<int32_t>(Rand()));
        }

        // Generate the next random number
        int32_t Next()
        {
            m_Seed = (a * m_Seed + c);
            return static_cast<int32_t>(m_Seed);
        }

        // Generate a random integer
        int32_t Rand()
        {
            return Next();
        }

        // Generate a random float in the range [0.0f, 1.0f)
        float NextFloat()
        {
            return static_cast<float>(Next()) / static_cast<float>(UINT32_MAX);
        }


        // Returns a random float number in the range [0, 1).
        float GetFraction()const 
        {
            MutateSeed();
            float Result;
            *(uint32_t*)&Result = 0x3F800000U | (m_Seed >> 9);
            return Result - 1.0f;
        }


        // Get the current seed
        int32_t GetCurrentSeed() const
        {
            return static_cast<int32_t>(m_Seed);
        }

        // Helper function for rand implementations.
        float FRand() const
        {
            return GetFraction();
        }

        // Helper function for rand implementations.
        int32_t RandHelper(int32_t A) const
        {
            return ((A > 0) ? static_cast<int32_t>(GetFraction() * static_cast<float>(A)) : 0);
        }

        // Helper function for rand implementations.
        int32_t RandRange(int32_t Min = (std::numeric_limits<int32_t>::min)(), int32_t Max = (std::numeric_limits<int32_t>::min)()) const
        {
            const int32_t Range = (Max - Min) + 1;
            return Min + RandHelper(Range);
        }

        // Helper function for rand implementations.
        float FRandRange(float min = (std::numeric_limits<int32_t>::min)(), float max = (std::numeric_limits<int32_t>::min)()) const
        {
            return min + (max - min) * FRand();
        }

    protected:

        // Mutate the current seed into the next seed.
        void MutateSeed() const
        {
            m_Seed = (m_Seed * 196314165U) + 907633515U;
        }

    private:

        // Holds the initial seed.
        int m_InitialSeed;

        // Holds the current seed.
        mutable uint32_t m_Seed;

        static constexpr int32_t a = 1664525;
        static constexpr int32_t c = 1013904223;
    };

}
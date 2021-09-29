//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once
#include <QObject>
#include <QList>
#include <QHash>
#include <QSharedPointer>
#include <QThread>
#include <vector>

#include "Generator.h"
#include "PatternType.h"

class GameOfLife : public Generator
{
    Q_OBJECT

    Q_PROPERTY(int rule READ getRule WRITE writeRule NOTIFY ruleChanged)
    Q_PROPERTY(PatternType patternType READ getPatternType WRITE writePatternType NOTIFY patternTypeChanged)

private:

    // Debugging
    bool  flagDebug = false;

    // computation variables here etc etc
    std::vector<double> cells;
    std::vector<double> temp_cells;

    // properties and rules
    int rule = 102;

    //pattern name
    PatternType GOLPattern = PatternType::Random;

    // iteration tracker
    double totalTime = 0;

    // GOL specific variables
    int currentGeneration;
    int lastGeneration;
    bool gameOver = false;

    int neighbors;


public:
     GameOfLife(int id, GeneratorMeta * meta);
    ~GameOfLife();

     void drawPattern(PatternType type);

    // overrides
    void computeIteration(double deltaTime) override;
    void initialize() override;
    double getLatticeValue(int x, int y) override;
    void writeLatticeValue(int x, int y, double value) override;

    // prop hooks
    int getRule();
    void writeRule(int rule);
    PatternType getPatternType() const;
    void writePatternType(PatternType GOLPattern);

signals:
    // QML signals
    void randSeedChanged(double randSeed);
    void ruleChanged(int rule);
    void patternTypeChanged(PatternType GOLPattern);
};

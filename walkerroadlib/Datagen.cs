using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace walkerroadlib
{
    public class ProtocolCommand 
    {
        public string Name {get; set;}
        public string Template {get; set;}
    }

    static public class Datagen
    {

        static public Dictionary<string, ProtocolCommand> CommandSet = new Dictionary<string, ProtocolCommand>
        {
            {"LOGIN", new ProtocolCommand {Name="LOGIN",Template="LOGIN {0}"} },
            {"SAY", new ProtocolCommand {Name="SAY",Template="SAY {0}"} },
            {"JOIN", new ProtocolCommand {Name="JOIN",Template="JOIN {0}"} },
        };

        static public string[] FirstNames = { "John", "Ben", "An", "Casey", "Ryan", "Justin", "Michael", "Cherry", "Luke", "Leia", "Vader", "Artoo", "Threepio", "Chewie", "Han", "Jarjar" };
        static public string[] Comments = { "A long time ago, in a galaxy far, far, away...", 
                                              "It is a period of civil war. Rebel spaceships, striking from a hidden base, have won their first victory against the evil Galactic Empire.", 
                                              "During the battle, Rebel spies managed to steal secret plans to the Empire's ultimate weapon, the Death Star, an armored space station with enough power to destroy an entire planet.", 
                                              "Pursued by the Empire's sinister agents, Princess Leia races home aboard her starship, custodian of the stolen plans that can save her people and restore freedom to the galaxy...", 
                                              "The awesome yellow planet of Tatooine emerges from a total eclipse, her two moons glowing against the darkness.", 
                                              "A tiny silver spacecraft, a Rebel Blockade Runner firing lasers from the back of the ship, races through space." ,
                                              "It is pursed by a giant Imperial Stardestroyer. Hundreds of deadly laserbolts streak from the Imperial Stardestroyer, causing the main solar fin of the Rebel craft to disintegrate.",
                                              "Did you hear that? They've shut down the main reactor. We'll be destroyed for sure. This is madness!",
                                              "There'll be no escape for the Princess this time.",
                                              "What's that?",
                                              "Hurry up! Come with me! What are you waiting for?! Get in gear!",
                                              "Artoo! Artoo-Detoo, where are you?",
                                              "At last! Where have you been?",
                                              "They're heading in this direction. What are we going to do? We'll be sent to the spice mine of Kessel or smashed into who knows what!",
                                              "Wait a minute, where are you going?",
                                              "The Death Star plans are not in the main computer.",
                                              "Where are those transmissions you intercepted?",
                                              "What have you done with those plans?",
                                              "We intercepted no transmissions. Aaah... This is a consular ship. Were on a diplomatic mission.",
                                              "If this is a consular ship... were is the Ambassador?",
                                              "Great shot, kid. That was one in a million.",
                                              "Remember, the Force will be with you... always.",
                                              "I have you now.",
                                              "You may fire when ready.",
                                              "Rebel base, in range.",
                                              "Commence primary ignition.",
                                              "The Force is strong with this one!",
                                              "Luke, trust me.",
                                              "Use the Force, Luke.",
                                              "I'm on the leader.",
                                              "Rebel base, thirty seconds and closing.",
                                              "It'll be just like Beggar's Canyon back home.",
                                              "Luke, at that speed will you be able to pull out in time?",
                                              "Hokey religions and ancient weapons are no match for a good blaster at your side, kid.",
                                              "You don't believe in the Force, do you?",
                                              "Kid, I've flown from one side of this galaxy to the other. I've seen a lot of strange stuff, but I've never seen anything to make me believe there's one all-powerful force controlling everything. There's no mystical energy field that controls my destiny.",
                                              "Remember, a Jedi can feel the Force flowing through him.",
                                              "You mean it controls your actions?",
                                              "Partially. But it also obeys your commands.",
                                              "Anyway, we should be at Alderaan about oh-two-hundred hours.",
                                              "Well, you can forget your troubles with those Imperial slugs. I told you I'd outrun 'em.",
                                              "I felt a great disturbance in the Force... as if millions of voices suddenly cried out in terror and were suddenly silenced. I fear something terrible has happened.",
                                              "I need your help, Luke. She needs your help. I'm getting too old for this sort of thing.",
                                              "I can't get involved! I've got work to do! It's not that I like the Empire. I hate it! But there's nothing I can do about it right now. It's such a long way from here.",
                                              "That's your uncle talking.",
                                              "Oh, God, my uncle. How am I ever going to explain this?",
                                              "Learn about the Force, Luke.",
                                              "Look, I can take you as far as Anchorhead. You can get a transport there to Mos Eisley or wherever you're going.",
                                              " You must do what you feel is right, of course.",
                                              "Until this battle station is fully operational we are vulnerable. The Rebel Alliance is too well equipped. They're more dangerous than you realize.",
                                              "Dangerous to your starfleet, Commander, not to this battle station!",
                                              "The Rebellion will continue to gain a support in the Imperial Senate as long as....",
                                              "Where am I? I must have taken a bad step...",
                                              "Can you stand? We've got to get out of here before the Sandpeople return.",
                                              "I don't think I can make it. You go on, Master Luke. There's no sense in you risking yourself on my account. I'm done for.",
                                              " No, you're not. What kind of talk is that?",
                                              "Quickly, son... they're on the move.",
                                              "No, my father didn't fight in the wars. He was a navigator on a spice freighter.",
                                              "That's what your uncle told you. He didn't hold with your father's ideals. Thought he should have stayed here and not gotten involved.",
                                              "You fought in the Clone Wars?",
                                              " Yes, I was once a Jedi Knight the same as your father.",
                                              "I wish I'd known him.",
                                              "He was the best star-pilot in the galaxy, and a cunning warrior. I understand you've become quite a good pilot yourself. And he was a good friend. Which reminds me...",
                                              "I have something here for you. Your father wanted you to have this when you were old enough, but your uncle wouldn't allow it. He feared you might follow old Obi-Wan on some damned-fool idealistic crusade like your father did.",
                                              "Sir, if you'll not be needing me, I'll close down for awhile.",
                                              "Your fathers lightsaber. This is the weapon of a Jedi Knight. Not as clumsy or as random as a blaster.",
                                              "An elegant weapon for a more civilized time. For over a thousand generations the Jedi Knights were the guardians of peace and justice in the Old Republic. Before the dark times, before the Empire.",
                                              "How did my father die?",
                                              "A young Jedi named Darth Vader, who was a pupil of mine until he turned to evil, helped the Empire hunt down and destroy the Jedi Knights. He betrayed and murdered your father. Now the Jedi are all but extinct. Vader was seduced by the dark side of the Force.",
                                              "The Force?",
                                              "Well, the Force is what gives a Jedi his power. It's an energy field created by all living things. It surrounds us and penetrates us. It binds the galaxy together.",
                                              "Now, let's see if we can't figure out what you are, my little friend. And where you come from.",
                                              "I saw part of the message he was...",
                                              "I seem to have found it.",
                                              "General Kenobi, years ago you served my father in the Clone Wars. Now he begs you to help him in his struggle against the Empire. I regret that I am unable to present my father's request to you in person, but my ship has fallen under attack and I'm afraid my mission to bring you to Alderaan has failed. I have placed information vital to the survival of the Rebellion into the memory systems of this R2 unit. My father will know how to retrieve it. You must see this droid safely delivered to him on Alderaan. This is our most desperate hour. Help me, Obi-Wan Kenobi, you're my only hope.",
                                              "You must learn the ways of the Force if you're to come with me to Alderaan.",
                                              "Alderaan? I'm not going to Alderaan. I've got to go home. It's late, I'm in for it as it is.",

                                              "Lord Vader, I should have known. Only you could be so bold. The Imperial Senate will not sit for this, when they hear you've attacked a diplomatic...",
                                              "Don't play games with me, Your Highness. You weren't on any mercy mission this time. You passed directly through a restricted system. Several transmissions were beamed to this ship by Rebel spies. I want to know what happened to the plans they sent you.",
                                              "I don't know what you're talking about. I'm a member of the Imperial Senate on a diplomatic mission to Alderaan...",
                                              "You're a part of the Rebel Alliance... and a traitor. Take her away!",
                                              "Holding her is dangerous. If word of this gets out, it could generate sympathy for the Rebellion in the senate.",
                                              "I have traced the Rebel spies to her. Now she is my only link to find their secret base!",
                                              "She'll die before she tells you anything.",
                                              "Leave that to me. Send a distress signal and then inform the senate that all aboard were killed!",
                                              "Lord Vader, the battle station plans are not aboard this ship! And no transmissions were made. An escape pod was jettisoned during the fighting, but no life forms were aboard.",
                                              "She must have hidden the plans in the escape pod. Send a detachment down to retrieve them. See to it personally, Commander. There'll be no one to stop us this time.",
                                              
                                              "How did I get into this mess? I really don't know how. We seem to be made to suffer. It's our lot in life.",
                                              "I've got to rest before I fall apart. My joints are almost frozen.",
                                          };

        static public string GetFirstName()
        {
            Random rnd = new Random();
            return FirstNames[rnd.Next(0, FirstNames.Count())];
        }

        static public string GetComment()
        {
            Random rnd = new Random();
            return Comments[rnd.Next(0, Comments.Count())];
        }
    }
}
